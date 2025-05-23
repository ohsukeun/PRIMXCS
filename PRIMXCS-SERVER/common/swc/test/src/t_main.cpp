/*****************************************************************************
 *                                                                           *
 * File Name : t_main.cpp                                                    *
 * Theme     : database plugin test main                                     *
 * Author    : Park, sung soo                                                *
 * History   : 1. created by  Park, sung soo '18. 01. 24                     *
 *                                                                           *
 *****************************************************************************/

#include <t_base.h>

#ifndef __T_MAIN_CPP__
#define __T_MAIN_CPP__
//-----------------------------------------------------------------------------
// FUNCTION DEFINITION
//-----------------------------------------------------------------------------
INT main(INT argc, CHAR** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	SetDebugModeOn();

	return RUN_ALL_TESTS();
}

//-----------------------------------------------------------------------------
#endif // __T_MAIN_CPP__
