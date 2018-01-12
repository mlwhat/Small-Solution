/***************************************************************************
code from 
***************************************************************************/
#pragma once
#include <chrono>


/*


int testFunc(){
	using namespace std::chrono;
	steady_clock::time_point t1 = steady_clock::now();

	// Do sth here.
	
	
	steady_clock::time_point t2 = steady_clock::now();


	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	std::cout << "It took me " << time_span.count() << " seconds.";

	return 0;
}


*/

namespace smallFrameWork
{
    /** Provides timer utilities to the application
    */
    class CpuTimer
    {
    public:
        using TimePoint = std::chrono::time_point < std::chrono::high_resolution_clock >;

        /** Returns the current time
        */
        static TimePoint getCurrentTimePoint()
        {
            return std::chrono::high_resolution_clock::now();
        }

        /** Update the timer.
            \return The TimePoint of the last update() call.
                This value is meaningless on it's own. Call CCpuTimer#calcDuration() to get the duration that passed between 2 TimePoints
        */
        TimePoint update()
        {
            auto now = getCurrentTimePoint();
            mElapsedTime = now - mCurrentTime;
            mCurrentTime = now;
            return mCurrentTime;
        }

        /** Get the time that passed from the last update() call to the one before that.
        */
        float getElapsedTime() const
        {
            return float(mElapsedTime.count());
        }

        /** Calculate the duration in milliseconds between 2 time points
        */
        static float calcDuration(TimePoint start, TimePoint end)
        {
            auto delta = end.time_since_epoch() - start.time_since_epoch();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(delta);
            return ((float)duration.count()) * 1.0e-6f;
        }

    private:
        TimePoint mCurrentTime;
        std::chrono::duration<double> mElapsedTime;
    };
}