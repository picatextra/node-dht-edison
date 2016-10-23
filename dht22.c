#include "mraa.h"

typedef int bool;
enum { false, true };

long readHighTime(mraa_gpio_context context, int timeout)
{
    int i;
    for (i = 0; i < timeout; i++)
        if (mraa_gpio_read(context) == 1)
            break;

    for (i = 0; i < timeout; i++)
        if (mraa_gpio_read(context) == 0)
            return i;

    return -1; //timeout
}

bool readDHT22(int pin, double *h, double *t)
{
    mraa_gpio_context sda;
    sda = mraa_gpio_init(pin);
    mraa_gpio_use_mmaped(sda, 1);

    mraa_gpio_dir(sda, MRAA_GPIO_OUT);
    mraa_gpio_write(sda, 0);
    usleep(1000);
    mraa_gpio_write(sda, 1);
    mraa_gpio_dir(sda, MRAA_GPIO_IN);

    char data[40];
    bool ret = 0;

    long treh = readHighTime(sda, 1000);
    //printf("treh=%d\n",treh);

    if (treh == -1)
    {
        fprintf(stderr, "timeout treh\n");
        return 0;
    }

    long th = 0;
    int i = 0;
    while ((th = readHighTime(sda, 1000)) != -1)
    {
        data[i++] = th;
    }

    if (th == -1 && i<40)
    {
        fprintf(stderr, "TIMEOUT i=%d\n", i);
        ret = false;
    }
    else
    {
        int th1 = 100;
        short st=0,sh=0,parity=0;

        for (i = 0; i < 16; i++)
            sh |= (data[i]>th1)<<(15-i);

        for (i = 17; i < 32; i++)
            st |= (data[i]>th1)<<(31-i);

        for (i = 32; i < 40; i++)
            parity |= (data[i]>th1)<<(39-i);
        
        printf("paste %d %d\n",sh,st);

         if(data[16]>th1)
        {
            printf("negative\n");
            // Convert to native negative format.
            st = -st;
        }

        short checksum = sh>>8 ;
        checksum+= sh&0xff;
        checksum+= st>>8;
        checksum+= st&0xff;

        ret = parity==(checksum&0xff);

        printf("checksum=%d  %X == %X\n",ret,parity,checksum);

        if(ret)
        {
            *h=sh/10.0;
            *t=st/10.0;
        }
    }

    mraa_gpio_close(sda);
    return ret;
}
/*
int main()
{
    double h,t;
    while(getchar()!='q')
    {
     readDHT22(31,&h,&t);
     printf("h=%f\nt=%f\n",h,t);
    }
    return 0;
}
*/