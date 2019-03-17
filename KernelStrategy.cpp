#include "KernelStrategy.h"
#include <vector>

void startExecuting()
{
  const std::string phraseToSay("Hello world");
  ALTextToSpeechProxy tts(getParentBroker());
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    tts.say(phraseToSay);
  }
}

bool KernelStrategy::updateGameState(int state) {
  currentGameState = gamecontroller::GameState(state);
  if (currentGameState == gamecontroller::GameState::FINISHED) {
    is_terminated_.store(true);
    return true;
  }
  if (currentGameState == gamecontroller::GameState::PLAYING) {

    std::thread main_thread([&](){
      startExecuting();
    });

    //https://en.cppreference.com/w/cpp/thread/thread/detach
    main_thread.detach();
    return true;
  }
}