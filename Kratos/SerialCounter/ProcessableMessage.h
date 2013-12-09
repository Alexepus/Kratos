#pragma once
#include <afxmt.h>
#include "AdamMessage.h"

typedef void (*TwoParamFunc)(AdamMessage message, void* param);

class ProcessableMessage
{
public:
	//ProcessableMessage(){}
	ProcessableMessage(AdamMessage message, TwoParamFunc processingFinishedCallback = nullptr, void* param = nullptr)
	{
		Message = message;
		ProcessingFinishedCallback = processingFinishedCallback;
		CallbackParam = param;
		FreeToDelete = (processingFinishedCallback != nullptr);
	}

	
	AdamMessage Message;
	TwoParamFunc ProcessingFinishedCallback;
	void* CallbackParam;
	AdamMessage Reply;
	CEvent ReplyReady;
	bool FreeToDelete;
};