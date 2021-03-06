 #include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/queue.h>

#include <rte_common.h>
#include <rte_memory.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_atomic.h>
#include <rte_branch_prediction.h>
#include <rte_ring.h>
#include <rte_log.h>
#include <rte_mempool.h>
#include <cmdline_rdline.h>
#include <cmdline_parse.h>
#include <cmdline_socket.h>
#include <cmdline.h>
#include <rte_mbuf.h>

#include <time.h> 

#include "allHeaders.h"

//#define RUNMAINDPDK
#ifdef RUNMAINDPDK
//This version runs in order of modules,which proceed in respective core

#define RTE_LOGTYPE_APP RTE_LOGTYPE_USER1
// #define MEMPOOL_F_SP_PUT         0x0

#define NUM_MBUFS 511
#define MBUF_CACHE_SIZE 32
static const char *MBUF_POOL = "MBUF_POOL";

static const char *Beforescramble = "Beforescramble";
static const char *scramble_2_BCC = "scramble_2_BCC";
static const char *BCC_2_modulation = "BCC_2_modulation";
static const char *modulation_2_CSD = "modulation_2_CSD";
static const char *IFFTAndaddWindow = "IFFTAndaddWindow";
static const char *AfterIFFT = "AfterIFFT";


const unsigned APEP_LEN_DPDK = 512; 

// static int i=0; 
struct rte_ring *Ring_Beforescramble;
struct rte_ring *Ring_scramble_2_BCC;
struct rte_ring *Ring_BCC_2_modulation;
struct rte_ring *Ring_modulation_2_CSD;
struct rte_ring *Ring_CSD_2_IFFTAndaddWindow;
struct rte_ring *Ring_AfterIFFT;
struct rte_mempool *mbuf_pool;
	
volatile int quit = 0;

long int ReadData_count = 0;
long int GenDataAndScramble_count = 0;
long int BCC_encoder_count = 0;
long int modulate__count = 0;
long int Data_CSD_count = 0;
long int CSD_encode_count = 0;
long int IFFTAndaddWindow_dpdk_count = 0;
long int CSD_encode_DPDK_count = 0;
int N_CBPS, N_SYM, ScrLength, valid_bits;

struct timespec time1,time2,time_diff;	/** < Test the running time. >*/
int time_test_flag = 0;
struct timespec diff(struct timespec start, struct timespec end);

static int ReadData(__attribute__((unused)) struct rte_mbuf *Data_In);
static int GenDataAndScramble_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In);
static int BCC_encoder_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In);
static int modulate_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In);
static int CSD_encode_dpdk (__attribute__((unused)) struct rte_mbuf *Data_In);
static int IFFTAndaddWindow_dpdk(__attribute__((unused)) struct rte_mbuf *Data_In);

static int ReadData_Loop();
static int GenDataAndScramble_Loop();
static int BCC_encoder_Loop();
static int modulate_Loop();
static int Data_CSD_Loop();  
static int IFFTAndaddWindow_loop();



struct timespec diff(struct timespec start, struct timespec end)
{
    struct  timespec temp;

     if ((end.tv_nsec-start.tv_nsec)<0) {
         temp.tv_sec = end.tv_sec-start.tv_sec-1;
         temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
     } else {
         temp.tv_sec = end.tv_sec-start.tv_sec;
         temp.tv_nsec = end.tv_nsec-start.tv_nsec;
     }
     return temp;
}

static int ReadData(__attribute__((unused)) struct rte_mbuf *Data) 
{
	//printf("sizeof(struct rte_mbuf) = %d\n", sizeof(struct rte_mbuf));
	//printf("Data->buflen = %d\n",Data->buf_len);
	//printf("Data->priv_size = %d\n",Data->priv_size);
	//printf("ReadData_count = %d\n", ReadData_count++);
	//printf("Data->data_off = %d\n",Data->data_off);
	if(time_test_flag==0){
		time_test_flag = 1;
		clock_gettime(CLOCK_REALTIME, &time1); //CLOCK_REALTIME:系统实时时间   
		//CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
		//CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间
		//CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间
	}
	FILE *fp=fopen("send_din_dec.txt","rt");
	unsigned char* databits=(unsigned char*)malloc(APEP_LEN_DPDK*sizeof(unsigned char));
	//unsigned char* databits_temp=(unsigned char*)malloc(APEP_LEN_DPDK*sizeof(unsigned char));
	if(databits == NULL){
		printf("error");
		return 0;
	}
	unsigned int datatmp=0;
	int i=0;
	for(i=0;i<APEP_LEN_DPDK;i++){
	    fscanf(fp,"%ud",&datatmp);
	    databits[i]=datatmp&0x000000FF;
	}
	memcpy(rte_pktmbuf_mtod(Data,unsigned char *), databits, APEP_LEN_DPDK);
	fclose(fp);
	free(databits);
	rte_ring_enqueue(Ring_Beforescramble, Data); //First half
	return 0;
}

static int GenDataAndScramble_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In) 
{
	//printf("GenDataAndScramble_DPDK_count = %d\n", GenDataAndScramble_DPDK_count++);
	unsigned char *databits = rte_pktmbuf_mtod(Data_In, unsigned char *);
	unsigned char *data_scramble = rte_pktmbuf_mtod_offset(Data_In, unsigned char *, RTE_MBUF_DEFAULT_BUF_SIZE*15);
	GenDataAndScramble(data_scramble, ScrLength, databits, valid_bits);	

	rte_ring_enqueue(Ring_scramble_2_BCC, Data_In); //The other half
	return 0;
}

static int BCC_encoder_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In)
{
	//printf("BCC_encoder_DPDK_count = %d\n", BCC_encoder_DPDK_count++);
	int CodeLength = N_SYM*N_CBPS/N_STS;
	unsigned char *data_scramble = rte_pktmbuf_mtod_offset(Data_In, unsigned char *, RTE_MBUF_DEFAULT_BUF_SIZE*15);
	unsigned char* BCCencodeout = rte_pktmbuf_mtod_offset(Data_In, unsigned char *, 0);

	BCC_encoder_OPT(data_scramble, ScrLength, N_SYM, &BCCencodeout, CodeLength);

	rte_ring_enqueue(Ring_BCC_2_modulation, Data_In); //First half
	return 0;
}

static int modulate_DPDK(__attribute__((unused)) struct rte_mbuf *Data_In)
{
	//printf("modulate_DPDK_count = %ld\n", modulate_DPDK_count++);
	unsigned char* BCCencodeout = rte_pktmbuf_mtod_offset(Data_In, unsigned char *, 0);
	complex32 *subcar_map_data = rte_pktmbuf_mtod_offset(Data_In, complex32 *, RTE_MBUF_DEFAULT_BUF_SIZE*15);

	modulate_mapping(BCCencodeout, &subcar_map_data);

	rte_ring_enqueue(Ring_modulation_2_CSD, Data_In);//First half
	return 0;
}	

static int CSD_encode_DPDK (__attribute__((unused)) struct rte_mbuf *Data_In)
{
	int i;
	CSD_encode_DPDK_count++;
	//printf("%ld\n", CSD_encode_DPDK_count++);//CSD_encode_DPDK_count = %ld\n
	complex32 *subcar_map_data = rte_pktmbuf_mtod_offset(Data_In, complex32 *, RTE_MBUF_DEFAULT_BUF_SIZE*15);
	complex32 *csd_data = rte_pktmbuf_mtod_offset(Data_In, complex32 *, 0);
	
	for(i=0;i<N_STS;i++){
		__Data_CSD_aux(&subcar_map_data, N_SYM, &csd_data,i);
	}
	
	
	if(CSD_encode_DPDK_count > 100000)
	{
		quit = 1;
		printf("ReadData_count=%ld\n",ReadData_count);
		printf("GenDataAndScramble_count=%ld\n",GenDataAndScramble_count);
		printf("BCC_encoder_count=%ld\n",BCC_encoder_count);
		printf("modulate__count=%ld\n",modulate__count);
		printf("CSD_encode_count=%ld\n",CSD_encode_count);

		clock_gettime(CLOCK_REALTIME, &time2);
		time_diff = diff(time1,time2);
		printf("CSD_encode_DPDK_count = %ld\n", CSD_encode_DPDK_count-1);
		printf("Start time # %.24s %ld Nanoseconds \n",ctime(&time1.tv_sec), time1.tv_nsec);
		printf("Stop time # %.24s %ld Nanoseconds \n",ctime(&time2.tv_sec), time2.tv_nsec);
		printf("Running time # %ld.%ld Seconds \n",time_diff.tv_sec, time_diff.tv_nsec);
		//printf("%.24s %ld Nanoseconds \n", ctime(&ts.tv_sec), ts.tv_nsec); 
	}
	
	//rte_ring_enqueue(Ring_CSD_2_IFFTAndaddWindow,Data_In);

	rte_mempool_put(Data_In->pool, Data_In);
	return 0;
}

static int IFFTAndaddWindow_dpdk(__attribute__((unused)) struct rte_mbuf *Data_In)
{
	int i;
	IFFTAndaddWindow_dpdk_count++;
	complex32 *csd_data = rte_pktmbuf_mtod_offset(Data_In, complex32 *, 0);
	complex32 *IFFT_data = rte_pktmbuf_mtod_offset(Data_In,complex32 *,RTE_MBUF_DEFAULT_BUF_SIZE*15);
	csd_data_IDFT(csd_data,IFFT_data,N_SYM);
  	
  	FILE *f=fopen("test-DataCSDbeforeIFFT.txt","w");
    for(i=0;i<subcar*N_STS*N_SYM;i++)
    {
   	fprintf(f,"(%hd,%hd)\n",csd_data[i].real,csd_data[i].imag);
    }
    fclose(f);	
	FILE *k=fopen("IFFT_data.txt","w");
	printStreamToFile_float(IFFT_data,5120,k);
	fclose(k);
	
	
	if(IFFTAndaddWindow_dpdk_count> 100)
	{
		quit = 1;
		printf("IFFTAndaddWindow_dpdk_count = %ld\n", IFFTAndaddWindow_dpdk_count-1);
		clock_gettime(CLOCK_REALTIME, &time2);
		time_diff = diff(time1,time2);

		printf("Start time # %.24s %ld Nanoseconds \n",ctime(&time1.tv_sec), time1.tv_nsec);
		printf("Stop time # %.24s %ld Nanoseconds \n",ctime(&time2.tv_sec), time2.tv_nsec);
		printf("Running time # %ld.%ld Seconds \n",time_diff.tv_sec, time_diff.tv_nsec);
		//printf("%.24s %ld Nanoseconds \n", ctime(&ts.tv_sec), ts.tv_nsec); 
	}


	rte_mempool_put(Data_In->pool, Data_In);
	return 0;
}

static int IFFTAndaddWindow_loop()
{
   void *Data_in_IFFTAndaddWindow=NULL;
   while (!quit)
   {
   		if(rte_ring_dequeue(Ring_CSD_2_IFFTAndaddWindow,&Data_in_IFFTAndaddWindow)<0)
   		{
   			continue;
   		}
   		else
   		{
   			IFFTAndaddWindow_dpdk(Data_in_IFFTAndaddWindow);
   		}
   }
   return 0;	
}

static int Data_CSD_Loop() 
{
	void *Data_In_CSD=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_modulation_2_CSD, &Data_In_CSD) < 0)
		{
			Data_CSD_count++;
			continue;
		}
		else 
		{
			CSD_encode_DPDK(Data_In_CSD);
		}
	
	}
	return 0;
}

static int modulate_Loop() 
{
	void *Data_In_modulate=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_BCC_2_modulation, &Data_In_modulate) < 0)
		{
			modulate__count++;
			continue;
		}
		else 
		{	
			modulate_DPDK(Data_In_modulate);
		}
	}	
	return 0;
}

static int BCC_encoder_Loop() 
{

	void *Data_In_BCC=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_scramble_2_BCC, &Data_In_BCC) < 0)
		{
			BCC_encoder_count++;
			continue;
		}
		else 
		{	
			BCC_encoder_DPDK(Data_In_BCC);
		}
	
	}
	return 0;
}

static int GenDataAndScramble_Loop() 
{
	void *Data_In_Scramble=NULL;
	while (!quit)
	{
		if (rte_ring_dequeue(Ring_Beforescramble, &Data_In_Scramble) < 0)
		{
			GenDataAndScramble_count++;
			continue;
		}
		else 
		{	
			GenDataAndScramble_DPDK(Data_In_Scramble);
		}
	
	}
	return 0;
}

static int ReadData_Loop() 
{
	struct rte_mbuf *Data =NULL;
	while (!quit){
		Data = rte_pktmbuf_alloc(mbuf_pool);
		if (Data == NULL)
		{
			ReadData_count++;
			continue;
		}
		else 
		{
			ReadData(Data);
		}
	
	}
	return 0;
}



int
main(int argc, char **argv)
{
	const unsigned flags = 0;
	const unsigned ring_size = 512;
	const unsigned pool_size = 512;
	const unsigned pool_cache = 32;
	const unsigned priv_data_sz = 0;
	int ret;
	int i;
	// 运行一次得到preamble和HeLTF.
	//generatePreambleAndHeLTF_csd();
	
    //complete one stream of Preamble
    unsigned char SigInfo[3];
	setSigInfo(SigInfo,3);
	complex32* basicSig=(complex32*)malloc(64*sizeof(complex32));
    MKSUREENMEM(basicSig);
    memset(basicSig,0,64*sizeof(complex32));
    generateBasicSig(basicSig,SigInfo,64);
	for(i=0;i<N_STS;i++){
        oneStreamOfSTF[i]=(complex32*)malloc(320*sizeof(complex32));
        oneStreamOfLTF[i]=(complex32*)malloc(320*sizeof(complex32));
        oneStreamOfSig[i]=(complex32*)malloc(160*sizeof(complex32));
        MKSUREENMEM(oneStreamOfSTF[i]);
        MKSUREENMEM(oneStreamOfLTF[i]);
        MKSUREENMEM(oneStreamOfSig[i]);
        memset(oneStreamOfSTF[i],0,320*sizeof(complex32));
        memset(oneStreamOfLTF[i],0,320*sizeof(complex32));
        memset(oneStreamOfSig[i],0,160*sizeof(complex32));
 		generateSTF(oneStreamOfSTF[i], i);
 		generateLTF(oneStreamOfLTF[i], i);
 		generateSig(basicSig,oneStreamOfSig[i],i);
	}
	/*FILE *t=fopen("oneStreamOfSTF.txt","w");
	for(i=0;i<N_STS;i++){
		printStreamToFile_float(oneStreamOfSTF[i],320,t);
	}
	fclose(t);

	FILE *s=fopen("oneStreamOfLTF.txt","w");
	for(i=0;i<N_STS;i++){
		printStreamToFile_float(oneStreamOfLTF[i],320,s);
	}
	fclose(s);

    FILE *p=fopen("oneStreamOfSig.txt","w");
	for(i=0;i<N_STS;i++){
		printStreamToFile_float(oneStreamOfSig[i],160,p);
	}
	fclose(p);*/

	// 运行一次得到比特干扰码表。
	Creatnewchart();
	// 运行一次得到BCC编码表。
	init_BCCencode_table();
	
	
	// 运行一次得到CSD表。
	//initcsdTableForHeLTF();
	// 初始化函数，计算OFDM符号个数，字节长度
	//int N_CBPS, N_SYM, ScrLength, valid_bits;
   	GenInit(&N_CBPS, &N_SYM, &ScrLength, &valid_bits);
   	// 运行一次得到生成导频的分流交织表
   	initial_streamwave_table(N_SYM);
   	init_mapping_table();
	///////////////////////////////////////////////////////////////////////////////////
	//unsigned lcore_id;
	ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Cannot init EAL\n");
		Ring_Beforescramble = rte_ring_create(Beforescramble , ring_size, rte_socket_id(), flags);
		Ring_scramble_2_BCC = rte_ring_create(scramble_2_BCC, ring_size, rte_socket_id(), flags);
		Ring_BCC_2_modulation = rte_ring_create(BCC_2_modulation, ring_size, rte_socket_id(), flags);
		Ring_modulation_2_CSD = rte_ring_create(modulation_2_CSD, ring_size, rte_socket_id(), flags);
		Ring_CSD_2_IFFTAndaddWindow=rte_ring_create(IFFTAndaddWindow,ring_size,rte_socket_id(),flags);
		Ring_AfterIFFT = rte_ring_create(AfterIFFT, ring_size, rte_socket_id(), flags);		
	    
	if (Ring_Beforescramble == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting sending ring\n");
	if (Ring_scramble_2_BCC == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting receiving ring\n");
	if (Ring_BCC_2_modulation == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting sending ring\n");
	if (Ring_modulation_2_CSD == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting receiving ring\n");
	if (Ring_CSD_2_IFFTAndaddWindow == NULL)
		rte_exit(EXIT_FAILURE,"Problem getting sending ring\n");
	if (Ring_AfterIFFT == NULL)
		rte_exit(EXIT_FAILURE, "Problem getting receiving ring\n");
	/* Creates a new mempool in memory to hold the mbufs. */
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE*30, rte_socket_id());

	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

	RTE_LOG(INFO, APP, "Finished Process Init.\n");

	rte_eal_remote_launch(ReadData_Loop, NULL,1);
	rte_eal_remote_launch(GenDataAndScramble_Loop, NULL, 2);
	rte_eal_remote_launch(BCC_encoder_Loop, NULL, 3);
	rte_eal_remote_launch(modulate_Loop, NULL, 4);
	rte_eal_remote_launch(Data_CSD_Loop, NULL,5);
	rte_eal_remote_launch(IFFTAndaddWindow_loop, NULL,6);
	rte_eal_mp_wait_lcore();
	return 0;
}
#endif // RUN

