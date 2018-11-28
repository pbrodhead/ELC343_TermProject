#include "project.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

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
        
        /* calculate frequency with correct casting */
        freq = SAM_SEC_FLOAT / maxCorrLoc;
        freq += 0.5;
        freqInt = (int) freq;
        
        /* output raw frequency value */
        sprintf(bottom, "freq: %d", freqInt);
        
        /* determine note from frequency, and output tuning */
        if(fabs(55 - freq) <= 0.1)
            sprintf(top, "A       ");
            
        else if((freq > 55) && (freq < 56.5))
            sprintf(top, "sharp A ");
            
        else if((freq > 56.5) && (freq < 58))
            sprintf(top, "flat A# ");
            
        else if(fabs(58 - freq) <= 0.1)
            sprintf(top, "A#      ");
            
        else if((freq > 58) && (freq < 60))
            sprintf(top, "sharp A#");
            
        else if((freq > 60) && (freq < 62))
            sprintf(top, "flat B  ");
            
        else if(fabs(62 - freq) <= 0.1)
            sprintf(top, "B       ");
            
        else if((freq > 62) && (freq < 63.5))
            sprintf(top, "sharp B ");
            
        else if((freq > 63.5) && (freq < 65.4))
            sprintf(top, "flat C  ");
            
        else if(fabs(65.4 - freq) <= 0.1)
            sprintf(top, "C       ");
            
        else if((freq > 65.4) && (freq < 67.4))
            sprintf(top, "sharp C ");
            
        else if((freq > 67.4) && (freq < 69.3))
            sprintf(top, "flat C# ");
            
        else if(fabs(69.3 - freq) <= 0.1)
            sprintf(top, "C#      ");
            
        else if((freq > 69.3) && (freq < 71.4))
            sprintf(top, "sharp C#");
            
        else if((freq > 71.4) && (freq < 73.4))
            sprintf(top, "flat D  ");
            
        else if(fabs(73.4 - freq) <= 0.1)
            sprintf(top, "D       ");
            
        else if((freq > 73.4) && (freq < 75.6))
            sprintf(top, "sharp D ");
            
        else if((freq > 75.6) && (freq < 77.8))
            sprintf(top, "flat D# ");
            
        else if(fabs(77.8 - freq) <= 0.1)
            sprintf(top, "D#      ");
            
        else if((freq > 77.8) && (freq < 80.1))
            sprintf(top, "sharp D#");
            
        else if((freq > 80.1) && (freq < 82.4))
            sprintf(top, "flat E  ");
            
        else if(fabs(82.4 - freq) <= 0.1)
            sprintf(top, "E       ");
            
        else if((freq > 82.4) && (freq < 84.9))
            sprintf(top, "sharp E ");
            
        else if((freq > 84.9) && (freq < 87.3))
            sprintf(top, "flat F  ");
            
        else if(fabs(87.3 - freq) <= 0.1)
            sprintf(top, "F       ");
            
        else if((freq > 87.3) && (freq < 89.9))
            sprintf(top, "sharp F ");
            
        else if((freq > 89.9) && (freq < 92.5))
            sprintf(top, "flat F# ");
            
        else if(fabs(92.5 - freq) <= 0.1)
            sprintf(top, "F#      ");
            
        else if((freq > 92.5) && (freq < 95.3))
            sprintf(top, "sharp F#");
           
        else if((freq > 95.3) && (freq < 98))
            sprintf(top, "flat G  ");
            
        else if(fabs(98 - freq) <= 0.1)
            sprintf(top, "G       ");
            
        else if((freq > 98) && (freq < 100.9))
            sprintf(top, "sharp G ");
            
        else if((freq > 100.9) && (freq < 103.8))
            sprintf(top, "flat G# ");
            
        else if(fabs(103.8 - freq) <= 0.1)
            sprintf(top, "G#      ");
            
        else if((freq > 103.8) && (freq < 106.9))
            sprintf(top, "sharp G#");
            
        else if((freq > 106.9) && (freq < 110))
            sprintf(top, "flat A  ");
            
        else if(fabs(110 - freq) <= 0.1)
            sprintf(top, "A       ");
        
        
        /* write to LCD screen */
        LCD_Position(0,0);
        LCD_PrintString(top);
        LCD_Position(1,0);
        LCD_PrintString(bottom);
    }
}