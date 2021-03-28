using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace GameLoop
{
    class PreciseTimer
    {
        [System.Security.SuppressUnmanagedCodeSecurity]
        [DllImport("Kernel32")]
        private static extern bool QueryPerformanceFrequency(ref long preformaceFrequency);

        [System.Security.SuppressUnmanagedCodeSecurity]
        [DllImport("Kernel32")]
        private static extern bool QueryPerformanceCounter(ref long preformaceCounter);

        private long _ticksPerSecond = 0;
        private long _previousElapsedTime = 0;

        public PreciseTimer()
        {
            QueryPerformanceFrequency(ref _ticksPerSecond);
            GetElapsedTime();
        }

        public double GetElapsedTime()
        {
            long time = 0;
            QueryPerformanceCounter(ref time);
            double elapsedTime = (double) (time - _previousElapsedTime)/(double) _ticksPerSecond;
            _previousElapsedTime = time;
            return elapsedTime;
        }
    }
}
