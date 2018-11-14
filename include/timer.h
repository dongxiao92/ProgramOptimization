#ifndef TIMER_H
#define TIMER_H
#include <chrono>

template <typename Unit=std::chrono::milliseconds>
class Timer{
public:
	using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
	using Clock = std::chrono::steady_clock;
	Timer():start_(false){}
	void start(){
		start_time_ = Clock::now();
		start_=true;
	}
	float getElapsedTime(){
		if(start_){
			auto duration = Clock::now() - start_time_;
			float time_val = static_cast<float>(std::chrono::duration_cast<Unit>(duration).count());
			start_ = false;
			return time_val;
		}
		return 0;
	}
private:
	bool start_;
	TimePoint start_time_;
};


#endif
