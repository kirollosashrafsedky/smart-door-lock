/******************************************************************************
 *
 * Project Name			: 	Door Lock System - HMI ECU
 *
 * File Name			: 	main.c
 *
 * Project Description	: 	Door Lock that supports
 * 							1.
 * 							2.
 *
 * Author				:	Kirollos Ashraf
 *
 *******************************************************************************/

/*******************************************************************************
 *                                Includes	                                   *
 *******************************************************************************/

/* header file */
#include "main.h"

/* for using app_init and app_update functions */
#include "App/app.h"

/*******************************************************************************
 *                           Function Definitions                              *
 *******************************************************************************/

/*
 * [Function Name]: main
 * [Function Description]: main app function
 * [Args]:
 * [in]: void
 * [Return]: int
 */
int main(void) {

	/* init the app */
	app_init();

	while(1)
	{
		/* update the app */
		app_update();
	}

	return 0;
}
