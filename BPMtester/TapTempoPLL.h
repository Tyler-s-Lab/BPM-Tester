#pragma once

#include <optional>
#include <vector>

class TapTempoPLL {
public:
    struct Config {
        double minBPM = 40.0;   // BPM 下限
        double maxBPM = 240.0;  // BPM 上限
        int    initTaps = 6;      // 初始化所需敲击数
        double alpha = 0.015;   // 相位增益 (0~1)
        double beta = 0.005;   // 周期增益 (0~1)
        double bpmSmooth = 0.3;   // BPM 输出低通系数 (0~1)
        double maxGapSec = 2.0;    // 超过该间隔视为重新开始
        double doubleTapRatio = 0.5;    // 间隔 < ratio*T 视为双敲
    };

    explicit TapTempoPLL(Config cfg = {});

    // 添加一个敲击时间戳（秒）。返回 true 表示本次更新有效。
    bool addTap(double t_sec);

    // 获取 BPM；未锁定返回 nullopt
    std::optional<double> getBPM() const;

    // 获取 BPM；未锁定时返回 fallback
    double getBPMOr(double fallback) const;

    // 是否已完成初始化并进入锁定跟踪状态
    bool isLocked() const;

    // 复位到空闲状态
    void reset();

private:
    enum class State {
        Idle, Collecting, Tracking
    };

    Config cfg_;
    State  state_ = State::Idle;
    double phase_ = 0.0;   // 下一个预测拍点时刻
    double period_ = 0.0;   // 当前周期（秒）
    double lastTap_ = 0.0;
    double bpmSmooth_ = 0.0;
    bool   bpmValid_ = false;
    std::vector<double> initBuf_;

    double clampPeriod(double T) const;
    bool   initPeriodFromBuf();
    static double median(std::vector<double> v);
};
