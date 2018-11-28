#include "project.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#define SAM_SEC 20000
#define SAM_SEC_FLOAT 20000.0
#define MINFREQ 55
#define MAXFREQ 110
#define M0 (SAM_SEC / MAXFREQ)
#define M1 (SAM_SEC / MINFREQ)
#define NUMSAMP 7200
#define N 6800


int main(void)
{
    //CyGlobalIntEnable; 
    char top[16];
    char bottom[16];
    int16_t curr  = 0;
    int16_t samples[NUMSAMP];
    int64_t corr[M1];
    int64_t maxCorr     = 0;
    uint16_t maxCorrLoc = 0;
    int sum             = 0;
    int mean            = 0;
    double freq         = 0.0;
    int freqInt         = 0;
    int numLoop         = 0;

    Clock_1_Start();
    ADC_1_Start();
    VDAC8_1_Start();
    LCD_Start();
    
    for(;;)
    {
        sum        = 0;
        maxCorr    = 0;
        maxCorrLoc = 0;
        
        /* get input from ADC and put into array */
        for(int i = 0; i < NUMSAMP; i++)   //should take 360 ms to get 72000 samples
        {
            ADC_1_IsEndConversion(ADC_1_WAIT_FOR_RESULT);
            curr = ADC_1_GetResult16();
            samples[i] = curr;
            sum += curr;
        }
        
        /* calculate mean of waveform */
        mean = sum / NUMSAMP;
        
        /* subtract off mean from input */
        for(int i = 0; i < NUMSAMP; i++)
        {
            samples[i] -= mean;
        }
        
        /* calculate autocorrelation of sample */
        for(int m = M0; m <= M1; m++)
        {
            corr[m] = 0;
            for(int n = 0; n < N; n++)
            {
                corr[m] += samples[n] * samples[n + m];
                if(corr[m] > maxCorr)
                {
                    maxCorr    = corr[m];
                    maxCorrLoc = m;
                }
            }
        }
        
        freq = SAM_SEC_FLOAT / maxCorrLoc;
        freq += 0.5;
        
        freqInt = (int) freq;
        numLoop++;
        //sprintf(top, "freq: %d      ", freqInt);
        sprintf(top, "maxCorrLoc: %d      ", maxCorrLoc);
        sprintf(bottom, "numLoop: %d", numLoop);
        
        LCD_Position(0,0);
        LCD_PrintString(top);
        LCD_Position(1,0);
        LCD_PrintString(bottom);
    }
}