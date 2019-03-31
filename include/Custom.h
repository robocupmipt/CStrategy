//
// Created by Vladislav Molodtsov on 2019-03-31.
//

#ifndef __CUSTOM_H__
#define __CUSTOM_H__

/* --------------------- Custom classes ------------------------ */

/*
 *
 * You can change these on your own.
 *
 */

/*
 *
 * Input class for trasnmission
 *
*/

#include"GCStates.h"

namespace message
{

class Receive
{
  public:

  gamecontroller::GameState state;
};

/*
 *
 * Output class for receiving
 *
*/

class Send
{
  public:

  gamecontroller::GameState state;
};

} /* namespace message */

#endif /* __CUSTOM_H__ */
