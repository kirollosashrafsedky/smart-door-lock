 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer-config.h
 *
 * Description: Config file for the BUZZER driver
 *
 * Author: Kirollos Ashraf
 *
 *******************************************************************************/

#ifndef __BUZZER_CONFIG_H__
#define __BUZZER_CONFIG_H__

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* number of buzzers used in the project.
 * used as a size for an array holding all initialized buzzers.
 * If BUZZERS_USED_COUNT = 1, the functions will not require the buzzer index
 * so pin number must be set below
 */
#define BUZZERS_USED_COUNT						1

#if BUZZERS_USED_COUNT == 1

/* the pin the buzzer is connected to
 * available only if BUZZERS_USED_COUNT = 1
 */
#define BUZZER_PIN								PB7

#endif /* BUZZERS_USED_COUNT == 1 */

#endif /* __BUZZER_CONFIG_H__ */
