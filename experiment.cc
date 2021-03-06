#include <rlglue/RL_glue.h>

#include "box.h"

typedef struct {
	double mean;
	double standard_dev;
}evaluation_point_t;

evaluation_point_t *evaluate_agent();
void single_evaluation();
void print_score(int afterEpisodes, evaluation_point_t *the_score);
void save_results_csv(evaluation_point_t *the_score[], const char *fileName);
void offline_demo();

int main(void){
	const char *task_spec;
	printf("\nThis is a RL Test program(Start)\n");
    
	task_spec = RL_init();
	printf("\nTASK SPEC : %s\n",task_spec);
   	printf("Starting offline demo\n----------------------------\nWill alternate learning for 25 episodes, then freeze policy and evaluate for 10 episodes.\n\n");
	printf("After Episode\tMean Return\tStandard Deviation\n-------------------------------------------------------------------------\n");
	offline_demo();

	RL_agent_message("save_policy results.dat");

	printf("\nCalling RL_cleanup and RL_init to clear the agent's memory...\n");

	RL_cleanup();
	RL_init();

	printf("Evaluating the agent's default policy:\n\t\tMean Return\tStandardDeviation\n------------------------------------------------------\n");
	single_evaluation();
	
	printf("\nLoading up the value function we saved earlier.\n");
	RL_agent_message("load_policy results.dat");

	printf("Evaluating the agent after loading the value function:\n\t\tMean Return\tStandardDeviation\n------------------------------------------------------\n");
	single_evaluation();
	
	RL_cleanup();
    
    printf("\nthis is a RL test prgram(End)\n");

    return 0;
}

void save_result_csv(evaluation_point_t *the_score[], const char *fileName){
	FILE *fp;
	int i=0;
	fp=fopen(fileName,"w");
	fprintf(fp, "#Results from SampleExperiment.c, First line is means, second line is standard derivations\n");
	for(i=0;i<21;i++){
		fprintf(fp, "%.2f", the_score[i]->mean);
	}
	fprintf(fp,"\n");
	for(i=0;i<21;i++){
		fprintf(fp,"%.2f", the_score[i]->standard_dev);
	}
	fprintf(fp,"\n");

	fclose(fp);
}

void offline_demo(){
	int i=0;
	int j=0;

	evaluation_point_t *this_score=0;
	evaluation_point_t *statistics[51];

	/* 학습 전 결과값 출력 */
	this_score = evaluate_agent();
	print_score(0,this_score);
	statistics[0]=this_score;

	for(i=0;i<50;i++){
		/* j번 학습 */
		for(j=0;j<500;j++){
			RL_episode(0);
		}

		/* 학습 후 결과값 출력 */
		this_score = evaluate_agent();
		print_score((i+1)*500,this_score);

		statistics[i+1]=this_score;
	}
	save_result_csv(statistics,"result.csv");

	for(i=0;i<51;i++){
		free(statistics[i]);
	}
}

/*
 이 함수는 learning없이 value_function 배열에 있는 값을 읽어서
 평균과 표준편차 값을 계산합니다.
*/
evaluation_point_t *evaluate_agent(){
	int i = 0;
	double sum = 0;
	double sum_of_squares = 0;
	double this_return = 0;
	double mean;
	double variance;
	int n = 100;
	evaluation_point_t *eval_point = 0;

	RL_agent_message("freeze learning");
	for(i=0;i<n;i++){
		RL_episode(0);
		this_return = RL_return();
		sum+=this_return;
		sum_of_squares+=this_return*this_return;
	}

	/* 평균과 표준편차를 계산합니다.*/
	mean = sum/(double)n;
	variance = (sum_of_squares - (double)n*mean*mean)/((double)n-1.0f);
	eval_point = (evaluation_point_t*)malloc(sizeof(evaluation_point_t));
	eval_point->mean = mean;
	eval_point->standard_dev = sqrt(variance);

	RL_agent_message("unfreeze learning");
	return eval_point;
}

/*
   한번 evaluate_agent 함수를 호출하고 결과값을 출력합니다.
*/
void single_evaluation(){
	evaluation_point_t *this_score=0;
	this_score = evaluate_agent();
	print_score(0, this_score);
	free(this_score);
}

/*
   evaluate_point_t 구조체에 있는 값을 출력합니다.
*/
void print_score(int afterEpisodes, evaluation_point_t *the_score){
	printf("%d\t\t%.2f\t\t%.2f\n", afterEpisodes, the_score->mean, the_score->standard_dev);
}
