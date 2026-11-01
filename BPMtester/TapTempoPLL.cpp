#include "TapTempoPLL.h"

#include <algorithm>
#include <cmath>

TapTempoPLL::TapTempoPLL(Config cfg) : cfg_(cfg) {
    reset();
}

void TapTempoPLL::reset() {
    state_ = State::Idle;
    phase_ = 0.0;
    period_ = 0.0;
    lastTap_ = 0.0;
    bpmSmooth_ = 0.0;
    bpmValid_ = false;
    initBuf_.clear();
}

bool TapTempoPLL::addTap(double t_sec) {
    // 1) 空闲：开始收集
    if (state_ == State::Idle) {
        initBuf_.clear();
        initBuf_.push_back(t_sec);
        lastTap_ = t_sec;
        state_ = State::Collecting;
        return false;
    }

    double dt = t_sec - lastTap_;
    if (dt <= 1e-6) return false;   // 时间必须严格递增

    // 2) 收集阶段
    if (state_ == State::Collecting) {
        double Tmin = 60.0 / cfg_.maxBPM;
        if (dt < cfg_.doubleTapRatio * Tmin) return false;   // 双敲/误敲

        initBuf_.push_back(t_sec);
        lastTap_ = t_sec;

        if (static_cast<int>(initBuf_.size()) >= cfg_.initTaps) {
            if (initPeriodFromBuf()) {
                state_ = State::Tracking;
                bpmSmooth_ = 60.0 / period_;
                bpmValid_ = true;
                return true;
            }
            // 初始化失败：丢掉最旧的点继续收集
            initBuf_.erase(initBuf_.begin());
        }
        return false;
    }

    // 3) 跟踪阶段
    if (dt > cfg_.maxGapSec) {          // 间隔太久：重新开始
        reset();
        return addTap(t_sec);
    }

    // 用当前 phase_ 估计敲击落在第几拍
    double    k_real = (t_sec - phase_) / period_;
    long long k = static_cast<long long>(std::llround(k_real));

    // 双敲：紧跟在上一拍附近且比预测还早
    if (k < 0) {
        if (dt < cfg_.doubleTapRatio * period_) {
            lastTap_ = t_sec;           // 只更新时间，不改状态
            return false;
        }
        k = 0;                          // tempo 剧变，保守处理
    }

    double t_pred = phase_ + static_cast<double>(k) * period_;
    double e = t_sec - t_pred;     // 相位误差

    // 限制单次误差，避免突发大偏差破坏锁相
    double half = 0.5 * period_;
    e = std::clamp(e, -half, half);

    // PLL 更新
    double denom = std::max(1.0, static_cast<double>(std::abs(k)) + 1.0); // 漏拍时误差分摊
    period_ += cfg_.beta * e / denom;
    period_ = clampPeriod(period_);

    // 推进到下一拍并施加相位修正
    phase_ = t_pred + period_ + cfg_.alpha * e;

    lastTap_ = t_sec;

    // BPM 输出平滑
    double bpm = 60.0 / period_;
    bpmSmooth_ = (1.0 - cfg_.bpmSmooth) * bpmSmooth_ + cfg_.bpmSmooth * bpm;
    bpmValid_ = true;
    return true;
}

std::optional<double> TapTempoPLL::getBPM() const {
    if (!bpmValid_) return std::nullopt;
    return bpmSmooth_;
}

double TapTempoPLL::getBPMOr(double fallback) const {
    return bpmValid_ ? bpmSmooth_ : fallback;
}

bool TapTempoPLL::isLocked() const {
    return state_ == State::Tracking;
}

double TapTempoPLL::clampPeriod(double T) const {
    double Tmin = 60.0 / cfg_.maxBPM;
    double Tmax = 60.0 / cfg_.minBPM;
    return std::clamp(T, Tmin, Tmax);
}

bool TapTempoPLL::initPeriodFromBuf() {
    if (initBuf_.size() < 2) return false;

    std::vector<double> iois;
    iois.reserve(initBuf_.size() - 1);
    for (size_t i = 1; i < initBuf_.size(); ++i) {
        double d = initBuf_[i] - initBuf_[i - 1];
        if (d > 0) iois.push_back(d);
    }
    if (iois.empty()) return false;

    std::sort(iois.begin(), iois.end());

    double med = median(iois);
    // 25% 分位数：若中位数明显接近它的 2 倍，可能存在漏敲
    double q25 = iois[iois.size() / 4];
    if (q25 > 0 && med > 1.6 * q25) med = q25;

    if (med <= 0) return false;
    period_ = clampPeriod(med);
    phase_ = initBuf_.back();   // 相位锁定在最后一次敲击
    return true;
}

double TapTempoPLL::median(std::vector<double> v) {   // 按值传，避免改动调用者
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    return (n % 2) ? v[n / 2] : 0.5 * (v[n / 2 - 1] + v[n / 2]);
}