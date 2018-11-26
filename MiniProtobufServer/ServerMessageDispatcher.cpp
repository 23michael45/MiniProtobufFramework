#include "ServerMessageDispatcher.h"
#include "SigninProcessor.h"
#include "MoveProcessor.h"

using namespace BaseCmd;

ServerMessageDispatcher::ServerMessageDispatcher()
{
	reqSignin reqSignin;
	m_ProcessorMap[reqSignin.GetTypeName()] = std::make_shared<SigninProcessor>();



	reqMove reqMove;
	m_ProcessorMap[reqMove.GetTypeName()] = std::make_shared<MoveProcessor>();


	reqAlarm reqAlerm;
	m_ProcessorMap[reqMove.GetTypeName()] = std::make_shared<MoveProcessor>();


}


ServerMessageDispatcher::~ServerMessageDispatcher()
{
}
