#ifndef		__MT_SERVICE_ROOM_DESK_TWO_H
#define		__MT_SERVICE_ROOM_DESK_TWO_H
#include "mtService.h"
#include "mtQueueHall.h"
#include "mtQueueUser.h"
#include "mtQueueMgr.h"

class mtServiceMgr;

/// 两人建桌子服务
class mtServiceRoomDeskTwo : public mtService
{
public:
	struct DataInit
	{
		long				    lStructBytes;
		mtQueueUser*	        pkQueueUser;
		mtQueuePacket*	        pkQueuePacket;
		mtServiceMgr*	        pkServiceMgr;
	};

	struct DataRead
	{
		long 							lStructBytes;			                 /// 包大小
		long                        	lServiceType;			                 /// 服务类型
		long 							plReservation[2];		                 /// 保留字段
		mtQueueUser::DataNode*	        pkQueueUserDataNode[MT_NODE_PLAYER - 1]; /// 人员信息
	} ;

	struct DataWrite
	{
		long 							lStructBytes;			/// 包大小
		long                        	lServiceType;			/// 服务类型
		long 							plReservation[2];		/// 保留字段
		long                            lResult;                /// 组桌子成功否 0失败，1成功
		DataUserInfo             		kUserInfo[2];           /// 其他玩家信息
	};

public:
	mtServiceRoomDeskTwo();
	virtual ~mtServiceRoomDeskTwo();

	virtual int init(void* pData);
	virtual int run(DataRun* pkDataRun);
	virtual int exit();

	int setWriteData(mtQueueUser::DataNode* pkDataNode1, mtQueueUser::DataNode* pkDataNode2, DataWrite* pkDataWrite);
	/// ai两个人组桌子服务响应
	int playTwoDeskAi(){ return 1;}
	int runTwoDeskWaitStartRecvAi(mtQueueUser::DataNode* pkQueueUserDataNode);

	mtQueueUser*	m_pkQueueUser;
	mtQueuePacket*	m_pkQueuePacket;
	mtServiceMgr*	m_pkServiceMgr;

};

#endif ///	__MT_SERVICE_ROOM_DESK_TWO_H