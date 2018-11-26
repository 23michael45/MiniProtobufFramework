#include "ClientMessageDispatcher.h"
#include "SigninProcessor.h"
#include "MoveProcessor.h"

using namespace BaseCmd;

ClientMessageDispatcher::ClientMessageDispatcher()
{
	rspSignin rspSignin;
	m_ProcessorMap[rspSignin.GetTypeName()] = std::make_shared<SigninProcessor>();


	rspMove rspMove;
	m_ProcessorMap[rspMove.GetTypeName()] = std::make_shared<MoveProcessor>();
}


ClientMessageDispatcher::~ClientMessageDispatcher()
{
}
