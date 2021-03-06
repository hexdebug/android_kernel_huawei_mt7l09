


/*****************************************************************************
  1 Include HeadFile
*****************************************************************************/
#include  "NasLmmPubMInclude.h"
#include  "NasMmlCtx.h"
#include  "NasEmmAttachEsmMsgProcess.h"
#include  "NasEmmAttDetInclude.h"
/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_NASEMMPLMN_C
/*lint +e767*/


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
  2 Declare the Global Variable
*****************************************************************************/



/*****************************************************************************
  3 Function
*****************************************************************************/

/*****************************************************************************
  3.1  消息发送 Function
*****************************************************************************/

/*lint -e960*/
/*lint -e961*/
VOS_UINT32  NAS_EMM_PLMN_SendAppStartCnf(
                    VOS_UINT32                              ulAppRslt,
                    VOS_UINT32                              ulOpId)
{
    LMM_MMC_START_CNF_STRU             *pstAppStartCnfMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulSendResult;

    /* 申请MMC内部消息 */
    pstAppStartCnfMsg  = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_START_CNF_STRU));

    if (NAS_EMM_PLMN_NULL_PTR == pstAppStartCnfMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendAppStartCnf: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    NAS_LMM_MEM_SET(pstAppStartCnfMsg,0,sizeof(LMM_MMC_START_CNF_STRU));

    /* 打包VOS消息头 */
    EMM_PUBU_COMP_MMC_MSG_HEADER((pstAppStartCnfMsg),
                             NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LMM_MMC_START_CNF_STRU));

    /* 填充消息ID */
    pstAppStartCnfMsg->ulMsgId          = ID_LMM_MMC_START_CNF;

    /* 填充消息体 */
    pstAppStartCnfMsg->ulOpId           = ulOpId;
    pstAppStartCnfMsg->ulRst            = ulAppRslt;

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstAppStartCnfMsg);

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}


VOS_UINT32  NAS_EMM_PLMN_SendAppStopCnf(
                    VOS_UINT32                              ulAppRslt,
                    VOS_UINT32                              ulOpId)
{
    LMM_MMC_STOP_CNF_STRU              *pstAppStopCnfMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulSendResult;

    (void)(ulAppRslt);

    /* 申请MMC内部消息 */
    pstAppStopCnfMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_STOP_CNF_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pstAppStopCnfMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendAppStopCnf: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }
    NAS_LMM_MEM_SET(pstAppStopCnfMsg,0,sizeof(LMM_MMC_STOP_CNF_STRU));

    /* 打包VOS消息头 */
    EMM_PUBU_COMP_MMC_MSG_HEADER((pstAppStopCnfMsg),
         NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LMM_MMC_STOP_CNF_STRU));

    /* 填充消息ID */
    pstAppStopCnfMsg->ulMsgId           = ID_LMM_MMC_STOP_CNF;

    /* 填充消息体 */
    pstAppStopCnfMsg->ulOpId            = ulOpId;

    /* 发送MMC消息 */
    NAS_LMM_SendLmmMmcMsg((VOS_VOID*)pstAppStopCnfMsg);

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}



VOS_UINT32  NAS_EMM_PLMN_SendRrcStartReq( VOS_VOID )
{
    LRRC_LMM_START_REQ_STRU            *pstRrcStartReqMsg;
    VOS_UINT32                          ulSendResult;


    /* 申请DOPRA消息 */
    pstRrcStartReqMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(LRRC_LMM_START_REQ_STRU));
    if(NAS_EMM_PLMN_NULL_PTR == pstRrcStartReqMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStartReq: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* 设置为0 */
    NAS_LMM_MEM_SET(pstRrcStartReqMsg,0,sizeof(LRRC_LMM_START_REQ_STRU));

    /* 打包VOS消息头 */
    NAS_EMM_PLMN_COMP_RRC_MSG_HEADER((pstRrcStartReqMsg),
                                      NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(LRRC_LMM_START_REQ_STRU));

    /* 填充消息ID */
    pstRrcStartReqMsg->enMsgId          = ID_LRRC_LMM_START_REQ;

    /* 填充消息体 */
    /* 填充卡状态 */
    NAS_EMM_SET_LRRCLMM_INF_USIM_STATE(pstRrcStartReqMsg->enSimStatus);


    pstRrcStartReqMsg->enLteRatPrio                             = NAS_LMM_GetCurLtePrio();


    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_GSM]     = NAS_LMM_GetCurGsmPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_WCDMA]   = NAS_LMM_GetCurWcdmaPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_LTE]     = NAS_LMM_GetCurLtePrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_CDMA1X]   = NAS_LMM_GetCurCdma1xPrio();
    pstRrcStartReqMsg->aucRatPrioList[NAS_LMM_RAT_TYPE_HRPD]     = NAS_LMM_GetCurHrpdPrio();

    pstRrcStartReqMsg->enUtranMode = NAS_LMM_GetEmmInfoUtranMode();

    /* 发送DOPRA消息 */
    NAS_LMM_SEND_MSG(                   pstRrcStartReqMsg);

    ulSendResult                        = NAS_EMM_OK;


    return ulSendResult;
}



VOS_UINT32  NAS_EMM_PLMN_SendRrcStopReq( VOS_VOID )
{
    RRC_MM_STOP_REQ_STRU                *pstRrcStopReqMsg;
    VOS_UINT32                          ulSendResult;

    /* 申请DOPRA消息 */
    pstRrcStopReqMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(RRC_MM_STOP_REQ_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pstRrcStopReqMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStopReq: MSG ALLOC ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    NAS_LMM_MEM_SET(pstRrcStopReqMsg,0,sizeof(RRC_MM_STOP_REQ_STRU));

    /* 打包VOS消息头 */
    NAS_EMM_PLMN_COMP_RRC_MSG_HEADER((pstRrcStopReqMsg),
                                    NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(RRC_MM_STOP_REQ_STRU))

    /* 填充消息ID */
    pstRrcStopReqMsg->enMsgId           = ID_LRRC_LMM_POWER_OFF_REQ;

    /*单板环境下，RRC消息队列中的消息超过10条，则以紧急消息形式发送关机消息*/
    #if (VOS_OS_VER != VOS_WIN32)
    if(VOS_ERR == VOS_CheckTaskQueue(PS_PID_ERMM, NAS_EMM_PLMN_10_MSG_IN_QUEUE))
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendRrcStopReq: Snd Urgent Msg!");

        /*如果RRC的消息队列超过10个消息，则以紧急消息方式发送关机消息*/
        NAS_LMM_SEND_URGENT_MSG(         pstRrcStopReqMsg);
    }
    else
    #endif
    {
        /*否则以普通消息形式发送*/
        NAS_LMM_SEND_MSG(                pstRrcStopReqMsg);
    }

    ulSendResult                        = NAS_EMM_OK;

    return ulSendResult;
}
VOS_UINT32  NAS_EMM_PLMN_SendMmcStartReq(
                    VOS_UINT32                              ulStartCause )
{
    EMMC_EMM_START_REQ_STRU              *pstMmcStartReqMsg;
    VOS_UINT32                          ulMsgLengthNoHeader;
    VOS_UINT32                          ulSendResult;

    /* 计算EMM要发给内部其它模块的内部消息长度, 不包含VOS头 */
    ulMsgLengthNoHeader = NAS_EMM_PLMN_INTRAMSG_LENGTH_NO_HEADER(EMMC_EMM_START_REQ_STRU);

    /* 获取内部消息地址 */
    pstMmcStartReqMsg = (EMMC_EMM_START_REQ_STRU *) NAS_LMM_GetSendIntraMsgBuffAddr(ulMsgLengthNoHeader);
    if (NAS_EMM_PLMN_NULL_PTR == pstMmcStartReqMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStartReq: GET INTRAMSG ADDR ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* 打包内部消息头 */
    NAS_EMM_PLMN_COMP_MM_INTRAMSG_HEADER((pstMmcStartReqMsg), ulMsgLengthNoHeader);

    /* 填充内部消息ID */
    pstMmcStartReqMsg ->enMsgId         = ID_EMMC_EMM_START_REQ;

    /* 填充消息体 */
    pstMmcStartReqMsg->enCause          = ulStartCause;

    /* PLMN子模块向MMC发送MMC_EMM_START_REQ内部消息 */
    ulSendResult =                      NAS_EMM_PLMN_OK;
    NAS_EMM_SEND_INTRA_MSG(             pstMmcStartReqMsg);

    return ulSendResult;
}
VOS_UINT32  NAS_EMM_PLMN_SendMmcStopReq( VOS_VOID )
{
    EMMC_EMM_STOP_REQ_STRU               *pstMmcStopReqMsg;
    VOS_UINT32                          ulMsgLengthNoHeader;
    VOS_UINT32                          ulSendResult;

    /* 计算EMM要发给内部其它模块的内部消息长度, 不包含VOS头 */
    ulMsgLengthNoHeader = NAS_EMM_PLMN_INTRAMSG_LENGTH_NO_HEADER(EMMC_EMM_STOP_REQ_STRU);

    /* 获取内部消息地址 */
    pstMmcStopReqMsg = (VOS_VOID *) NAS_LMM_GetSendIntraMsgBuffAddr(ulMsgLengthNoHeader);
    if (NAS_EMM_PLMN_NULL_PTR == pstMmcStopReqMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq: GET INTRAMSG ADDR ERROR!!!");
        return NAS_EMM_PLMN_ERR;
    }

    /* 打包内部消息头 */
    NAS_EMM_PLMN_COMP_MM_INTRAMSG_HEADER((pstMmcStopReqMsg), ulMsgLengthNoHeader);

    /* 填充内部消息ID */
    pstMmcStopReqMsg->enMsgId           = ID_EMMC_EMM_STOP_REQ;

    ulSendResult =                      NAS_EMM_PLMN_OK;
    NAS_EMM_SEND_INTRA_MSG(             pstMmcStopReqMsg);

    return ulSendResult;
}

VOS_VOID NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_ENUM_UINT32 ulStatRst)
{
    EMM_ESM_STATUS_IND_STRU             *pEmmEsmStatMsg;

    NAS_EMM_PUBU_LOG1_NORM("NAS_EMM_PLMN_SendEmmEsmStautsInd", ulStatRst);

    /* 申请DOPRA消息 */
    pEmmEsmStatMsg = (VOS_VOID *) NAS_LMM_ALLOC_MSG(sizeof(EMM_ESM_STATUS_IND_STRU));
    if (NAS_EMM_PLMN_NULL_PTR == pEmmEsmStatMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendEmmEsmStautsInd: MSG ALLOC ERROR!!!");
        return;
    }

    /* 打包VOS消息头 */
    NAS_EMM_PLMN_COMP_ESM_MSG_HEADER(pEmmEsmStatMsg,
                                    NAS_EMM_PLMN_MSG_LENGTH_NO_HEADER(EMM_ESM_STATUS_IND_STRU));

    /* 填充消息ID */
    pEmmEsmStatMsg->ulMsgId             = ID_EMM_ESM_STATUS_IND;

    /*填充消息内容*/
    pEmmEsmStatMsg->enEMMStatus         = ulStatRst;

    /*clear global:EpsContextStatusChange*/
    if(ulStatRst == EMM_ESM_ATTACH_STATUS_DETACHED)
    {
        NAS_EMM_GetEpsContextStatusChange() = NAS_EMM_EPS_BEARER_STATUS_NOCHANGE;
    }
    /*向ESM发送ID_EMM_ESM_STATUS_IND消息*/

    NAS_LMM_SEND_MSG(                   pEmmEsmStatMsg);

    return;

}

VOS_VOID NAS_EMM_PLMN_ImsiPagingProc(VOS_VOID)
{
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulRst;

    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_PLMN_ImsiPagingProc is entered!");


    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_DEREG;
    EmmState.enSubState                 = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /*向APP发送APP_MM_DETACH_IND消息*/
    NAS_EMM_MmcSendDetIndLocal(MMC_LMM_L_LOCAL_DETACH_IMSI_PAGING);
    /* 本地DETACH释放资源:动态内存、赋初值 */
    NAS_LMM_DeregReleaseResource();

    /* 删除GUTI后会自动保存EPS_LOC,所以需要先设置STATUS */
    NAS_LMM_GetMmAuxFsmAddr()->ucEmmUpStat = EMM_US_NOT_UPDATED_EU2;

    /*删除GUTI,KSIasme,TAI list,LVR TAI*/
    NAS_EMM_ClearRegInfo(NAS_EMM_NOT_DELETE_RPLMN);

    /*send INTRA ATTACH REQ*/
    ulRst = NAS_EMM_SendIntraAttachReq();
    if(NAS_EMM_PLMN_OK != ulRst)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_ImsiPagingProc: send INTRA ATTACH REQ ERR !");
    }

    return;
}

VOS_VOID NAS_EMM_PLMN_STmsiPagingProc(VOS_VOID)
{
    VOS_UINT32                          ulCurEmmStat;
    NAS_LMM_EMM_PLMN_LOG_INFO(           "NAS_EMM_PLMN_STmsiPagingProc is entered!");

    ulCurEmmStat                        = NAS_LMM_PUB_COMP_EMMSTATE(
                                        NAS_EMM_CUR_MAIN_STAT,
                                        NAS_EMM_CUR_SUB_STAT);

    if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE))
    {
        NAS_EMM_SER_RcvRrcStmsiPagingInd();
    }
    else if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND))
    {
        if(NAS_EMM_SUCC != NAS_EMM_JudgeBarType(NAS_EMM_BAR_TYPE_MT))
        {
            NAS_EMM_SER_RcvRrcStmsiPagingInd();
        }
    }
    else if (ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        /* 记录UPDATE_MM标识 */
        /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

        NAS_EMM_SER_RcvRrcStmsiPagingInd();
    }
    else
    {
       NAS_LMM_EMM_PLMN_LOG_WARN(    "NAS_EMM_PLMN_STmsiPagingProc:Not process the paging with S-TMSI:NAS State Error");
    }

    return;
}



VOS_VOID NAS_EMM_PLMN_CsPagingProc
(
    LRRC_LNAS_PAGING_UE_ID_ENUM_UINT32 enPagingUeId
)
{
    VOS_UINT32                          ulCurEmmStat;
    NAS_LMM_EMM_PLMN_LOG_INFO(           "NAS_EMM_PLMN_CsPagingProc is entered!");

    ulCurEmmStat                        = NAS_LMM_PUB_COMP_EMMSTATE(
                                        NAS_EMM_CUR_MAIN_STAT,
                                        NAS_EMM_CUR_SUB_STAT);
    if(ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_NORMAL_SERVICE))
    {
        NAS_EMM_SER_RcvRrcCsPagingInd(enPagingUeId);
    }
    else if(ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_WAIT_ACCESS_GRANT_IND))
    {
        if(NAS_EMM_SUCC != NAS_EMM_JudgeBarType(NAS_EMM_BAR_TYPE_MT))
        {
            NAS_EMM_SER_RcvRrcCsPagingInd(enPagingUeId);
        }
    }
    /* REG ATTEMPING_TO_UPDATE_MM态下CS PAGING需上报给MM,解决UPDATE MM态下不处理CS PAGING问题,提高用户体验 */
    else if(ulCurEmmStat == NAS_LMM_PUB_COMP_EMMSTATE( EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        NAS_EMM_SER_RegAttempToUpdateMmRcvRrcCsPagingInd(enPagingUeId);
    }
    else
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(    "NAS_EMM_PLMN_CsPagingProc:NAS State cannot process cs paging!");
    }
    return;
}

/*****************************************************************************
 Function Name   : NAS_EMM_StartRatIsValid
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:非法
                   NAS_LMM_SUCC:合法
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_StartRatIsValid(const MMC_LMM_START_REQ_STRU *pstAppStartReq)
{
    RRMM_RAT_PRIO_ENUM_UINT8    usGsmRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_GSM];
    RRMM_RAT_PRIO_ENUM_UINT8    usWcdmaRatPrio = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_WCDMA];
    RRMM_RAT_PRIO_ENUM_UINT8    usLteRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_LTE];
    RRMM_RAT_PRIO_ENUM_UINT8    usCdma1xRatPrio = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_CDMA1X];
    RRMM_RAT_PRIO_ENUM_UINT8    usHrpdRatPrio   = pstAppStartReq->aucRatPrioList[RRMM_RAT_TYPE_HRPD];

    if (  (RRMM_RAT_PRIO_BUTT <= usGsmRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usWcdmaRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usLteRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usCdma1xRatPrio)
        ||(RRMM_RAT_PRIO_BUTT <= usHrpdRatPrio) )
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_StartRatIsExist: MMC START REQ RAT LIST IS VALID ");
        return NAS_LMM_FAIL;
    }

    return NAS_LMM_SUCC;
}
/*****************************************************************************
 Function Name   : NAS_EMM_RatListIsValid
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:非法
                   NAS_LMM_SUCC:合法
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_RatListIsValid(RRMM_RAT_PRIO_ENUM_UINT8    usGsmRatPrio,
                                            RRMM_RAT_PRIO_ENUM_UINT8    usWcdmaRatPrio,
                                            RRMM_RAT_PRIO_ENUM_UINT8    usLteRatPrio)
{
    if (  (RRMM_RAT_PRIO_BUTT <= usGsmRatPrio)
        &&(RRMM_RAT_PRIO_BUTT <= usWcdmaRatPrio)
        &&(RRMM_RAT_PRIO_BUTT <= usLteRatPrio) )
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_StartRatIsExist: MMC START REQ RAT LIST IS VALID ");
        return NAS_LMM_FAIL;
    }

    return NAS_LMM_SUCC;
}

/*****************************************************************************
 Function Name   : NAS_EMM_lteRatIsOnlyExist
 Description     :
 Input           :
 Output          :
 Return          : NAS_LMM_FAIL:接入技术不只有L
                   NAS_LMM_SUCC:接入技术只有L
 History         :
    1.ZHANGCAIXIA      2011-9-9  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_lteRatIsOnlyExist(VOS_VOID)
{
    if (  ( (NAS_LMM_RAT_PRIO_NULL < NAS_LMM_GetCurLtePrio())&&(NAS_LMM_RAT_PRIO_BUTT >NAS_LMM_GetCurLtePrio()))
    &&( (NAS_LMM_RAT_PRIO_NULL == NAS_LMM_GetCurGsmPrio())||(NAS_LMM_RAT_PRIO_BUTT == NAS_LMM_GetCurGsmPrio()))
    &&( (NAS_LMM_RAT_PRIO_NULL == NAS_LMM_GetCurWcdmaPrio())||(NAS_LMM_RAT_PRIO_BUTT ==NAS_LMM_GetCurWcdmaPrio() )))
    {
        return NAS_LMM_SUCC;
    }

    NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_lteRatIsExist: MMC START REQ RAT LIST LTE IS NOT ONLY EXIST ");
    return NAS_LMM_FAIL;
}


/*****************************************************************************
  3.2  开机相关流程处理 Function
*****************************************************************************/


/*****************************************************************************
 Function Name   : NAS_EMM_MmcStartCheck
 Description     : MMC_LMM_START_REQ_STRU入参检查，若检查成功，将开机参数转换为
                   EMM自己的开机参数
 Input           : pstAppStartReq
 Output          : pstEmmStartInfo
 Return          : NAS_LMM_FAIL: 检查不通过
                   NAS_LMM_SUCC: 检查通过

 History         :
    1.HanLufeng 41410      2011-3-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MmcStartCheck(
                const MMC_LMM_START_REQ_STRU          *pstAppStartReq,
                      NAS_EMM_START_INFO_STRU         *pstEmmStartInfo)
{
    VOS_UINT32 ulCnt                    = NAS_COMM_NULL;


    /* EMM自己保存的接入技术赋初始值 */
    for(ulCnt = 0; ulCnt < NAS_LMM_RAT_TYPE_BUTT; ulCnt++)
    {
        pstEmmStartInfo->aucRatPrioList[ulCnt] = NAS_LMM_RAT_PRIO_BUTT;
    }

    /* 如果介入技术列表都为空，则返回错误 */
    if(NAS_LMM_FAIL == NAS_EMM_StartRatIsValid(pstAppStartReq))
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MmcStartCheck: MMC START REQ RAT LIST IS NULL ");
        return  NAS_LMM_FAIL;
    }

    /* 保存接入技术 */
    for(ulCnt = 0; ulCnt < NAS_LMM_RAT_TYPE_BUTT; ulCnt++)
    {
        pstEmmStartInfo->aucRatPrioList[ulCnt] = pstAppStartReq->aucRatPrioList[ulCnt];
    }

    /*--------转换为LMM自己的待机类型 NAS_EMM_RAT_TYPE_ENUM_UINT8 -------*/
    if(MMC_LMM_TRANSCEIVER_TYPE_SINGLE == pstAppStartReq->ulTransceiverType)
    {
        pstEmmStartInfo->ulTransceiverType = NAS_EMM_TRANSCEIVER_TYPE_SINGLE;
    }
    else if(MMC_LMM_TRANSCEIVER_TYPE_DUAL == pstAppStartReq->ulTransceiverType)
    {
        pstEmmStartInfo->ulTransceiverType = NAS_EMM_TRANSCEIVER_TYPE_DUAL;
    }
    else
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MmcStartCheck: ulTransceiverType = ",
                                 pstAppStartReq->ulTransceiverType);
        return  NAS_LMM_FAIL;
    }



    /*--------检查卡状态参数 -------*/
    if( (MMC_LMM_USIM_VALID   != pstAppStartReq->ulUsimState) &&
        (MMC_LMM_USIM_INVALID != pstAppStartReq->ulUsimState))
    {
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MmcStartCheck: ulUsimState = ",
                                 pstAppStartReq->ulUsimState);
        return  NAS_LMM_FAIL;
    }

    return  NAS_LMM_SUCC;


}
VOS_UINT32    NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_EMM_FSM_STATE_STRU              stEmmState;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;
    VOS_UINT32                          ulSendResult;
#if (VOS_OS_VER != VOS_WIN32)
    FTM_TMODE_ENUM                      enMspFtmMode = EN_FTM_TMODE_SIGNAL ;
#endif
    NAS_EMM_START_INFO_STRU             stEmmStartInfo;
    VOS_UINT32                          ulRst;

    (VOS_VOID)ulMsgId;

    pstAppStartReq                      = pMsg;

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq is entered.");

    /* 入参检查，若成功，转换接口开机参数类型到EMM的参数类型 */
    ulRst = NAS_EMM_MmcStartCheck(pstAppStartReq, &stEmmStartInfo);
    if(NAS_LMM_FAIL ==  ulRst)
    {   /* 若出错，通知MMC开机失败  */

        /* 向APP发送APP_MM_START_CNF消息*/
        ulRst = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL,
                                    pstAppStartReq->ulOpId);
        if (NAS_EMM_PLMN_OK != ulRst)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : Send Msg Failure!!! ulSendOuterMsgResult=",
                                     ulRst);
        }

        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq: MSG IE ERROR!!!");
        return  NAS_LMM_MSG_HANDLED;
    }

    /*EMM&MMC 全局变量清空 */
    NAS_LMM_EmmAndMmcGlobInfoClear();

    /* 将保存上报信息的全局变量清除 */
    NAS_LMM_MEM_SET(&g_stEmmOmInfo, 0, sizeof(OM_EMM_STATE_INFO_STRU));

    /* EMM Info 赋初始值：与USIM/NV无关的赋值为有效值；
    与USIM/NV有关的，赋值为无效值，等读USIM/NV时再重新赋值为有效值 */
    NAS_LMM_EmmInfoInit();

    /* 保存开机MMC下发的UTRAN模式 */
    NAS_LMM_SetEmmInfoUtranMode(pstAppStartReq->enUtranMode);

    /* 各子模块控制变量赋初始值 */
    NAS_LMM_EmmSubModGlobInit();

    /* 设置App消息的相关参数 */
    /*如果收到的OpId为0xffff,则StartReq为收到ErrInd后发起的内部消息 */
    /* 将保存参数移至发挂起之前 */
    if (NAS_EMM_INTRA_APP_START_OPID != pstAppStartReq->ulOpId)
    {
        NAS_EMM_SaveAppMsgPara(pstAppStartReq->ulMsgId,pstAppStartReq->ulOpId);

        /* 保存开机消息中的各种UE模式信息，以待给RRC发送开机消息时使 */
        NAS_EMM_SAVE_START_INFO(stEmmStartInfo);
    }
    /* V7R1，开机LNAS处于挂起态，也要通知ESM和ERABM挂起，不等响应 */
    /* 发送 ESM 挂起消息*/
    NAS_EMM_SendEsmSuspendInd();

    /* 发送 ERABM 挂起消息*/
    NAS_EMM_SendErabmSuspendInd();

#if (VOS_OS_VER != VOS_WIN32)
    /*产线上无卡模式，则关闭安全*/
    if((LPS_OM_GetTmode(&enMspFtmMode)== ERR_MSP_SUCCESS)&&( EN_FTM_TMODE_SIGNAL_NOCARD == enMspFtmMode))
    {
        NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : EN_FTM_TMODE_SIGNAL_NOCARD, Set USIM feature as soft!");
        g_ulSmcControl = NAS_EMM_SMC_CLOSE;
    }
#endif

    /*读IMSI成功，才认为有卡，部分USIM文件是MMC读取的，LMM从MMC那里读到LMM中来 */
    if((MMC_LMM_USIM_VALID == pstAppStartReq->ulUsimState)
       &&(NAS_LMM_USIM_READ_SUCC == NAS_LMM_ReadMmcUsimFile()))
    {
        /* 设置保存在EMM中的卡状态全局变量 */
        NAS_EMM_SetUsimState(NAS_LMM_SIM_STATUS_AVAILABLE);
        /* 然后再读取LTE自己的文件 */
        NAS_LMM_ReadLteUsimFile();

        /* 启动TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
        NAS_LMM_StartStateTimer(TI_NAS_LMM_TIMER_WAIT_USIM_CNF);

        /* 将状态转移至MS_NULL + SS_WAIT_MMC_START_CNF状态 */
        stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
        stEmmState.enMainState                = EMM_MS_NULL;
        stEmmState.enSubState                 = EMM_SS_NULL_WAIT_READING_USIM;
        stEmmState.enStaTId                   = TI_NAS_LMM_TIMER_WAIT_USIM_CNF;
        NAS_LMM_StaTransProc(stEmmState);

        return  NAS_LMM_MSG_HANDLED;
    }

    /*卡不可用，给MMC/RRC发送开机消息，进入无卡开机*/
    NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitAppStartReqMsgAppStartReq : Hard Usim UNAVAILABLE:");

    /* 设置保存在EMM中的卡状态全局变量 */
    NAS_EMM_SetUsimState(NAS_LMM_SIM_STATUS_UNAVAILABLE);


    /* 向MMC发送MMC_EMM_START_REQ(正常启动)消息 */
    ulSendResult = NAS_EMM_PLMN_SendMmcStartReq(EMMC_EMM_START_REQ_CAUSE_NORMAL_START);
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStartReq : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    /* 启动TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_WAIT_MMC_START_CNF状态 */
    stEmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState                = EMM_MS_NULL;
    stEmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_START_CNF;
    stEmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER;
    NAS_LMM_StaTransProc(stEmmState);

    return NAS_LMM_MSG_HANDLED;
}

#if (VOS_OS_VER == VOS_WIN32)
/*****************************************************************************
 Function Name   : NAS_EMM_SendMmcErrInd
 Description     :
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.HanLufeng 41410      2011-3-4  Draft Enact

*****************************************************************************/
VOS_VOID  NAS_EMM_SendMmcErrInd( VOS_UINT32 ulErrTpye )
{

    LMM_MMC_ERR_IND_STRU      *pLmmMmcErrIndMsg    = NAS_EMM_NULL_PTR;

    NAS_LMM_PUBM_LOG_INFO( "NAS_EMM_SendMmcErrInd: enter. RESET.");

    /*申请MMC内部消息 */
    pLmmMmcErrIndMsg = (VOS_VOID *)NAS_LMM_GetLmmMmcMsgBuf(sizeof(LMM_MMC_ERR_IND_STRU));

    if(NAS_EMM_NULL_PTR == pLmmMmcErrIndMsg)
    {
        NAS_LMM_PUBM_LOG_ERR( "NAS_EMM_SendMmcErrInd: MSG ALLOC ERR !!");
        return;
    }

    NAS_LMM_MEM_SET(pLmmMmcErrIndMsg,0,sizeof(LMM_MMC_ERR_IND_STRU));

    /* 填写LMM_MMC_ERR_IND_STRU 的DOPRA消息头 */
    EMM_PUBU_COMP_MMC_MSG_HEADER(pLmmMmcErrIndMsg,
            sizeof(LMM_MMC_ERR_IND_STRU) - EMM_LEN_VOS_MSG_HEADER);

    /* 填写EMM_RABM_RRC_CON_REL_IND 的消息ID标识 */
    pLmmMmcErrIndMsg->ulMsgId           = ID_LMM_MMC_ERR_IND;

    /* 发送消息 */
    NAS_LMM_SendLmmMmcMsg(                    pLmmMmcErrIndMsg);

    return;

}
#endif


VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq is entered.");

    pstAppStartReq = pMsg;

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgAppStartReq: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 向APP发送APP_MM_START_CNF消息(回复开机成功，已经开机) */
    ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_SUCC, pstAppStartReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_WhenStopingThenMsgAppStartReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    MMC_LMM_START_REQ_STRU             *pstAppStartReq;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_WhenStopingThenMsgAppStartReq is entered.");

    pstAppStartReq                      = pMsg;

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_WhenStopingThenMsgAppStartReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 说明已经收到关机消息，但关机的整个过程没有完成，还没有回复APP_MM_STOP_CNF
    消息，直接回复APP开机失败－－APP_MM_START_CNF消息（回复开机失败，RRC在关机）*/
    ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL, pstAppStartReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf is entered.");

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);


    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStartCnfMsgMmcStartCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*检查卡状态，当卡无效则重置全局变量*/
    if (NAS_LMM_SIM_STATUS_UNAVAILABLE == NAS_LMM_GetSimState())
    {
        NAS_EMM_NoUsimInit();
    }

    /* 停止TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_MMC_START_CNF_TIMER);

    /* 向RRC发送RRC_MM_START_REQ消息 */
    ulSendResult = NAS_EMM_PLMN_SendRrcStartReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendRrcStartReq : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    /* 启动TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_RRC_START_CNF状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_START_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;


    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStartCnfMsgTimerExp: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* 向APP发送APP_MM_START_CNF消息（回复开机失败，等待MMC启动超时） */
        ulSendResult = NAS_EMM_PLMN_SendAppStartCnf(MMC_LMM_FAIL,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendResult=",
                                     ulSendResult);
            return NAS_LMM_MSG_HANDLED;
        }
    }

    /*清空APP参数*/
    NAS_EMM_ClearAppMsgPara();

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_APP_START_REQ状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendOuterMsgResult;
    VOS_UINT32                          ulSendInnerMsgResult;
    VOS_UINT32                          ulAppRslt;
    NAS_EMM_FSM_STATE_STRU              stEmmState;
    LRRC_LNAS_START_RESULT_ENUM_UINT32    enResult;
    NAS_LMM_SIM_STAT_ENUM_UINT32        ulSimState;
    LRRC_LMM_START_CNF_STRU               *pstRrcMmStartCnfMsg = (LRRC_LMM_START_CNF_STRU*)(pMsg);

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(stEmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStartCnfMsgRrcStartCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 停止TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_RRC_START_CNF_TIMER);

    /* 处理RRCMmStartCnfMsg启动结果*/
    enResult                            = pstRrcMmStartCnfMsg->enResult;
    if(LRRC_LNAS_START_SUCCESS == enResult)
    {
        ulAppRslt = MMC_LMM_SUCC;
    }
    else
    {
        ulAppRslt = MMC_LMM_FAIL;
        /*如果开机失败，则直接让封装底软的接口reboot modem以便保存现场*/
        NAS_EMM_SendMmcErrInd(NAS_EMM_REBOOT_TYPE_RRC_START_FAIL);
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* 向APP发送APP_MM_START_CNF消息*/
        ulSendOuterMsgResult = NAS_EMM_PLMN_SendAppStartCnf(ulAppRslt,
                                    NAS_EMM_GetAppMsgOpId());
        if (NAS_EMM_PLMN_OK != ulSendOuterMsgResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendOuterMsgResult=",
                                     ulSendOuterMsgResult);
        }
    }

    /*清空APP参数*/
    NAS_EMM_ClearAppMsgPara();

    /* 启动结果为RRC_NAS_START_SUCCESS的处理*/
    if(LRRC_LNAS_START_SUCCESS == enResult)
    {
        /*根据卡是否存在，转移EMM的状态*/
        ulSimState                          = NAS_LMM_GetSimState();

        if(NAS_LMM_SIM_STATUS_AVAILABLE == ulSimState)
        {
            /* 将状态转移至MS_DEREG + SS_PLMN_SEARCH状态 */
            stEmmState.enMainState          = EMM_MS_DEREG;
            stEmmState.enSubState           = EMM_SS_DEREG_NO_CELL_AVAILABLE;
            stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
        }
        else
        {
            /* 下发内部UsimStatusInd消息，通知卡无效，发起Any小区搜索 */
            /*NAS_EMMC_SendIntraUsimStatusInd(USIMM_CARD_STATUS_ABSENT);*/

            /* 将状态转移至MS_DEREG + SS_PLMN_SEARCH状态 */
            stEmmState.enMainState          = EMM_MS_DEREG;
            stEmmState.enSubState           = EMM_SS_DEREG_NO_IMSI;
            stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
        }
    }

    /* 启动结果不为RRC_NAS_START_SUCCESS的处理*/
    else
    {
        /* 向MMC发送MMC_EMM_STOP_REQ消息 */
        ulSendInnerMsgResult = NAS_EMM_PLMN_SendMmcStopReq();
        if (NAS_EMM_PLMN_ERR == ulSendInnerMsgResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq : Send IntraMsg Failure!!!");
        }

        /* 将状态转移至EMM_MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ状态 */
        stEmmState.enMainState          = EMM_MS_NULL;
        stEmmState.enSubState           = EMM_SS_NULL_WAIT_APP_START_REQ;
        stEmmState.enStaTId             = TI_NAS_EMM_STATE_NO_TIMER;
    }

    /* 改变状态 */
    NAS_LMM_StaTransProc(stEmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32  NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp(
                VOS_UINT32              ulMsgId,
                const VOS_VOID         *pMsg )
{
    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return( NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_START_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStartCnfMsgTimerExp: STATE ERROR!!!");
        return(NAS_LMM_MSG_DISCARD);
    }

    /*如果开机失败，则直接让封装底软的接口reboot modem以便保存现场*/
    NAS_EMM_SendMmcErrInd(NAS_EMM_REBOOT_TYPE_RRC_START_WAIT_RRC_TIMEOUT_FAIL);

    return(NAS_LMM_SUCC);
}



/*****************************************************************************
  3.3  关机相关流程处理 Function
*****************************************************************************/

VOS_UINT32    NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;
    MMC_LMM_STOP_REQ_STRU              *pstAppStopReq;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq is entered.");

    pstAppStopReq                       = pMsg;

    /*停止所有定时器*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsAnyReadUsimStateMsgAppStopReq: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 将状态转移至MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ状态 */
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* 向APP发送APP_MM_STOP_CNF消息（回复关机成功，已经关机） */
    ulSendResult = NAS_EMM_PLMN_SendAppStopCnf(MMC_LMM_SUCC,pstAppStopReq->ulOpId);
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulRet;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR                    == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return(                         NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(  EMM_MS_NULL, EMM_SS_NULL_WAIT_SWITCH_OFF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(       "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 停止TI_NAS_EMM_PTL_SWITCH_OFF_TIMER */
    NAS_LMM_StopPtlTimer(               TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    ulRet                               = NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd();

    return(                             ulRet);
}
VOS_VOID NAS_EMM_ProcLocalStop( VOS_VOID )
{

    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */
    VOS_UINT8                           ucImsiRefreshFlag;
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */
    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /*向ESM发送EMM_ESM_STATUS_IND消息*/
    NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);

    /*通知 RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();

    /* 通知ETC:REL_IND */
    NAS_EMM_TcSendRelInd();

    /* 如果是DEREG态，EMM参数无需重复写卡了，在进入DEREG态时已写安全上下文，在
       链路释放时已写PSLOC */
    if (EMM_MS_DEREG != NAS_EMM_CUR_MAIN_STAT)
    {
        /* 如果current安全上下文为mapped 且non-current native上下文存在，
           将后者设置为current，同时删除current mapped，以便关机时写入卡中
           依据:      24301协议     5.5.2.2.1*/
        NAS_EMM_SecuDeregClrSecuCntxt();

        /* 写入EMM NV相关信息 */
        /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */
        ucImsiRefreshFlag = NAS_MML_GetImsiRefreshStatus();
        NAS_LMM_SndOmImsiRefreshStatus(ucImsiRefreshFlag);
        /* IMSI REFRESH 情况下不写卡 */
        if(VOS_FALSE == ucImsiRefreshFlag)
        {
            NAS_EMM_WriteNvMmInfo();
        }
        /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,End */
    }

    /* 向MMC发送MMC_EMM_STOP_REQ消息 */
    ulSendResult = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_ProcLocalStop : Send Msg Failure!!!");
        return;
    }

    /* 启动TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_MMC_STOP_CNF状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);
    return;
}


VOS_UINT32    NAS_EMM_MsNotNullNotRegMsgAppStopReq(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    MMC_LMM_STOP_REQ_STRU              *pstAttStopReq;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullNotRegMsgAppStopReq is entered.");

    pstAttStopReq                       = pMsg;

    /*停止所有定时器*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* 设置App消息的相关参数 */
    NAS_EMM_SaveAppMsgPara(pstAttStopReq->ulMsgId,pstAttStopReq->ulOpId);

    NAS_EMM_ProcLocalStop();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStopCnfMsgMmcStopCnf: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 清除本地资源 */
    /*NAS_EMM_ClearLocalResource();关机不清内存不读卡，等再次收到 START消息后再清内存，并读卡 */

    /* 停止TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* 向RRC发送RRC_MM_STOP_REQ消息 */
    ulSendResult = NAS_EMM_PLMN_SendRrcStopReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendRrcStopReq : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
    }

    /* 启动TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_RRC_STOP_CNF状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_MMC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 向RRC发送RRC_MM_STOP_REQ消息 */
    ulSendResult = NAS_EMM_PLMN_SendRrcStopReq();
    if (NAS_EMM_PLMN_OK != ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_MsNullSsWaitMmcStopCnfMsgTimerExp : Send Msg Failure!!! ulSendResult=",
                                 ulSendResult);
        return NAS_LMM_MSG_HANDLED;
    }

    /* 启动TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_RRC_STOP_CNF状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_RRC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    VOS_UINT32                          ulSendResult;

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStopCnfMsgRrcStopCnf: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 停止TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER */
    NAS_LMM_StopStateTimer(TI_NAS_EMM_WAIT_RRC_STOP_CNF_TIMER);

    /* 状态迁移到 Null.WtAppStartReq*/
    NAS_EMM_ProcRrcStopRslt();

    /* 向APP发送APP_MM_STOP_CNF消息（回复关机成功，正常关机） */
    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_STOP_REQ))
    {

        ulSendResult = NAS_EMM_PLMN_SendAppStopCnf(APP_SUCCESS,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                     ulSendResult);
        }
    }
    /* 清除关机标识,该标志提供给LRRC,用于LRRC判断空口是否是关机detach 该标志
       在LMM收到MMC的stop req时置为1 */
    NAS_LMM_SetEmmInfoLtePowerOffFlag(NAS_EMM_NO);
    /*清除APP参数*/
    NAS_EMM_ClearAppMsgPara();

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    VOS_UINT32                          ulResult;

    (VOS_VOID)ulMsgId;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_NULL, EMM_SS_NULL_WAIT_RRC_STOP_CNF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNullSsWaitRrcStopCnfMsgTimerExp: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 状态迁移到 Null.WtAppStartReq*/
    NAS_EMM_ProcRrcStopRslt();

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_STOP_REQ))
    {
        /* 向APP发送APP_MM_STOP_CNF消息（回复关机失败，等待RRC停止超时） */
        ulResult = NAS_EMM_PLMN_SendAppStopCnf(MMC_LMM_FAIL,NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStopCnf : Send Msg Failure!!! ulSendResult=",
                                     ulResult);
        }
    }
    /* 清除关机标识,该标志提供给LRRC,用于LRRC判断空口是否是关机detach 该标志
       在LMM收到MMC的stop req时置为1 */
    NAS_LMM_SetEmmInfoLtePowerOffFlag(NAS_EMM_NO);
    /*清除APP参数*/
    NAS_EMM_ClearAppMsgPara();

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
  3.4  其它相关流程处理 Function
*****************************************************************************/

VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulRslt;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*清除BAR标识*/
    NAS_EMM_ClearBarResouce();

    /* 判断若为挂起态，则通知ESM和ERABM激活，并获取GU的安全参数 */
    ulRslt = NAS_EMM_NoCellSuspendMsgMmcPlmnReq();
    if (NAS_LMM_MSG_HANDLED == ulRslt)
    {
        NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcPlmnInd: MMC ACTIVE LNAS");
        return NAS_LMM_MSG_HANDLED;
    }

    /*如果后续支持连接态下搜网，此处需增加判断USIM卡是否有效:如果卡无效且不存在连接，
      状态转换到DEREG+NO_IMSI，清除相应的资源通知MMC卡无效；如果卡无效且存在连接，
      状态转换到DEREG+WAIT_MRRC_REL+_CNF，发送RRC_MM_REL_REQ消息，等收到RRC_MM_REL_CNF
      消息后，状态转换到DEREG+NO_IMSI,清除相应的资源通知MMC卡无效*/

    /* 判断EMM当前状态*/
    /* 针对GCF9.2.3.1.5 用例临时方案 ,UE丢网,MMC发起搜网 ,搜网期间T3412超时,发起的TAU类型是combined TAU with IMSI类型，
       而仪器测试中,存在现网干扰下,UE在现网上受限驻留,导致无法转到no cell态。所以针对GCF测试此处EMM不转状态 */
    if((PS_SUCC == LPS_OM_IsTestMode())
        &&(EMM_MS_REG == EmmState.enMainState)
        && (EMM_SS_REG_LIMITED_SERVICE == EmmState.enSubState))
    {
        return NAS_LMM_MSG_HANDLED;
    }

    /* 针对GCF9.2.3.1.5 用例，UE丢网，MMC发起搜网，搜网期间T3412超时，发起的TAU类型是combined TAU with IMSI类型的*/
    if ((EMM_MS_REG == EmmState.enMainState)
        && (EMM_SS_REG_NO_CELL_AVAILABLE == EmmState.enSubState))
    {
        return NAS_LMM_MSG_HANDLED;

    }
    else if(EMM_MS_REG == EmmState.enMainState)
    {
        /* 将状态转移至MS_REG + SS_REG_PLMN_SEARCH状态 */
        EmmState.enMainState        = EMM_MS_REG;
        EmmState.enSubState         = EMM_SS_REG_PLMN_SEARCH;
        EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;

    }
    else
    {
        if (EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        {
            /* 通知ESM紧接着EMM还要做紧急注册，ESM不需要清除动态表中的紧急PDN建立记录 */
            NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_EMC_ATTACHING);
        }
        else
        {
            /*向ESM发送ATTACH结果*/
            NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);
        }
        /* 将状态转移至MS_DEREG + SS_DEREG_PLMN_SEARCH状态 */
        EmmState.enMainState        = EMM_MS_DEREG;
        EmmState.enSubState         = EMM_SS_DEREG_PLMN_SEARCH;
        EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;
    }

    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                     *pMsg
)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*清除BAR标识*/
    NAS_EMM_ClearBarResouce();

    /* 记录UPDATE_MM标识 */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

    /* 将状态转移至MS_REG + SS_REG_PLMN_SEARCH状态 */
    EmmState.enMainState        = EMM_MS_REG;
    EmmState.enSubState         = EMM_SS_REG_PLMN_SEARCH;
    EmmState.enStaTId           = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd
 Description     : 处理在REG+ATTEMPT_TO_UPDATE_MM状态下收到内部链路释放消息
 Input           : ulMsgId-------------------------消息ID
                   pMsgStru------------------------消息指针
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-17  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd
(
    VOS_UINT32                          ulMsgId,
    const VOS_VOID                     *pMsg
)
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd is entered.");
    (VOS_VOID)ulMsgId;

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgIntraMrrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*发送内部消息 INTRA_CONN2IDLE_REQ,更新连接状态为IDLE*/
    NAS_EMM_CommProcConn2Ilde();

    return NAS_LMM_MSG_HANDLED;
}
/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd
 Description     : 在reg.normal态下内部释放处理
 Input           : None
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.sunbing49683      2010-12-29  Draft Enact

*****************************************************************************/
VOS_UINT32  NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd is entered.");
    (VOS_VOID)ulMsgId;

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsNormalServiceMsgMrrcRelInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*发送内部消息 INTRA_CONN2IDLE_REQ,更新连接状态为IDLE*/
    NAS_EMM_CommProcConn2Ilde();

    return NAS_LMM_MSG_HANDLED;
}



VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;
    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgMmcCoverageLostInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*清除BAR标识*/
    NAS_EMM_ClearBarResouce();

    /* 关闭当前EMM的除Del Forb Ta Proid之外的状态定时器, Del Forb Ta Proid只能在关机时停止*/
    NAS_LMM_StopAllStateTimerExceptDelForbTaProidTimer();

    /* 如果当前CSFB延时定时器在运行，说明在REG-NORMAL态下释放过程中收到CSFB，
       但是在释放后搜小区出现丢网，此时应触发去GU搜网继续CSFB */
    if(NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_CSFB_DELAY))
    {
        /* 停止CSFB时延定时器 */
        NAS_LMM_StopPtlTimer(TI_NAS_EMM_PTL_CSFB_DELAY);

        /* 给MMC上报SERVICE失败触发搜网去GU */
        NAS_EMM_MmcSendSerResultIndOtherType(MMC_LMM_SERVICE_RSLT_FAILURE);
    }

    if ((EMM_MS_REG         == EmmState.enMainState) ||
        (EMM_MS_TAU_INIT    == EmmState.enMainState) ||
        (EMM_MS_SER_INIT    == EmmState.enMainState)) /* 已经注册 */
    {
        /* 将状态转移至MS_REG + SS_NO_CELL_AVAILABLE状态 */
        EmmState.enMainState            = EMM_MS_REG;
        EmmState.enSubState             = EMM_SS_REG_NO_CELL_AVAILABLE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    }
    else
    {
        /*向ESM发送ID_EMM_ESM_STATUS_IND消息*/
        NAS_EMM_PLMN_SendEmmEsmStautsInd(      EMM_ESM_ATTACH_STATUS_DETACHED);

        /* 将状态转移至MS_DEREG + SS_NO_CELL_AVAILABLE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_NO_CELL_AVAILABLE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    }
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}

/*****************************************************************************
 Function Name   : NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd
 Description     : 在REG+ATTEMPT_TO_UPDATE_MM状态下收到丟网消息
 Input           : ulMsgId----------------------消息ID
                   pMsg-------------------------丟网消息指针
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.lihong00150010      2011-11-16  Draft Enact
*****************************************************************************/
VOS_UINT32 NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd
(
    VOS_UINT32                          ulMsgId,
    VOS_VOID                           *pMsg
)
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_REG, EMM_SS_REG_ATTEMPTING_TO_UPDATE_MM))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsRegSsRegAttemptUpdateMmMsgMmcCoverageLostInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*清除BAR标识*/
    NAS_EMM_ClearBarResouce();

    /* 关闭当前EMM的除Del Forb Ta Proid之外的状态定时器, Del Forb Ta Proid只能在关机时停止*/
    NAS_LMM_StopAllStateTimerExceptDelForbTaProidTimer();

    /* 记录UPDATE_MM标识 */
    /*NAS_LMM_SetEmmInfoUpdateMmFlag(NAS_EMM_UPDATE_MM_FLAG_VALID);*/

    /* 将状态转移至MS_REG + SS_NO_CELL_AVAILABLE状态 */
    EmmState.enMainState            = EMM_MS_REG;
    EmmState.enSubState             = EMM_SS_REG_NO_CELL_AVAILABLE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    NAS_LMM_FSM_STATE_STRU               stEmmState;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd;
    VOS_UINT32                          ulSendResult;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    PS_MEM_SET(&stCurTa, 0, sizeof(NAS_MM_TA_STRU));

    (VOS_VOID)ulMsgId;

    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd is entered.");

    /*check the input params*/
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /*get the sys info*/
    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    /*判断当前小区是否被禁止*/
    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo)
         || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /*向ESM发送EMM_ESM_STATUS_IND消息*/
        NAS_EMM_PLMN_SendEmmEsmStautsInd(EMM_ESM_ATTACH_STATUS_DETACHED);


        stEmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
        stEmmState.enMainState            = EMM_MS_DEREG;
        stEmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        stEmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(stEmmState);

        NAS_EMM_SetAttType();

        /*向MMC发送LMM_MMC_ATTACH_CNF或LMM_MMC_ATTACH_IND消息*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
        return NAS_LMM_MSG_HANDLED;
    }


    /* 获取当前TA和上次尝试注册的TA信息 */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* 判断是否需要等定时器超时再发起注册 */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: The same as last attempt TA.");
        if (   (NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3411))
            || (NAS_LMM_TIMER_RUNNING == NAS_LMM_IsPtlTimerRunning(TI_NAS_EMM_PTL_T3402)) )
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*重设attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    stEmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
    stEmmState.enMainState            = EMM_MS_DEREG;
    stEmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    stEmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(stEmmState);

    /*send Msg EMM_INTRA_ATTACH_REQ to ATTACH module*/
    ulSendResult = NAS_EMM_SendIntraAttachReq();

    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    EMMC_EMM_SYS_INFO_IND_STRU          *pstMmcSysInfoInd;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    VOS_UINT32                          ulSendResult;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NORMAL_SERVICE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;
	/* lihong00150010 emergency tau&service begin */
    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }
	/* lihong00150010 emergency tau&service end */
    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA在禁止的TA列表中 */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /* 将状态转移至MS_DEREG + SS_DEREG_LIMITED_SERVICE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*向MMC发送LMM_MMC_ATTACH_CNF或LMM_MMC_ATTACH_IND消息*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
    }
    /*PC REPLAY MODIFY BY LEILI BEGIN*/
    if(NAS_EMM_AUTO_ATTACH_NOT_ALLOW == NAS_EMM_GetPsAttachAllowFlg())
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd : attach not allowed!!!");
        return NAS_LMM_MSG_HANDLED;
    }
    /*PC REPLAY MODIFY BY LEILI END*/
    /* 向ATTACH模块发送EMM_INTRA_ATTACH_REQ消息 */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNormalSrvMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    PS_MEM_SET(&stCurTa, 0, sizeof(NAS_MM_TA_STRU));
    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd is entered.");

    /* 状态匹配检查,若不匹配,退出 */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_LIMITED_SERVICE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA在禁止的TA列表中 */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        /* 将状态转移至MS_DEREG + SS_DEREG_LIMITED_SERVICE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*向MMC发送LMM_MMC_ATTACH_CNF或LMM_MMC_ATTACH_IND消息*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);

        return NAS_LMM_MSG_HANDLED;
    }

    /* 获取当前TA和上次尝试注册的TA信息 */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* 判断是否需要等定时器超时再发起注册 */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd: The same as last attempt TA.");

        if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*重设attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    /* 将状态转移至MS_DEREG + SS_DEREG_NORMAL_SERVICE状态 */
    EmmState.enMainState            = EMM_MS_DEREG;
    EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* 向ATTACH模块发送EMM_INTRA_ATTACH_REQ消息 */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsLimitSrvMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd(
                                            VOS_UINT32              ulMsgId,
                                            VOS_VOID               *pMsg )
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    PS_MEM_SET(&stCurTa, 0, sizeof(NAS_MM_TA_STRU));
    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd is entered.");

    /* 状态匹配检查,若不匹配,退出 */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_PLMN_SEARCH))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA在禁止的TA列表中 */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {
        NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: FORBID SYS INFO");

        /* 将状态转移至MS_DEREG + SS_DEREG_LIMITED_SERVICE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*向MMC发送LMM_MMC_ATTACH_CNF或LMM_MMC_ATTACH_IND消息*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);

        return NAS_LMM_MSG_HANDLED;
    }

    /* 获取当前TA和上次尝试注册的TA信息 */
    NAS_EMM_GetCurrentTa(&stCurTa);
    pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

    /* 判断是否需要等定时器超时再发起注册 */
    if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
    {
        NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd: The same as last attempt TA.");

        if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
        {
            return NAS_LMM_MSG_HANDLED;
        }
    }
    else
    {
        /*重设attach attempt counter*/
        NAS_EMM_AttResetAttAttempCounter();
    }

    /* 将状态转移至MS_DEREG + SS_DEREG_NORMAL_SERVICE状态 */
    EmmState.enMainState            = EMM_MS_DEREG;
    EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
    EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    /* 向ATTACH模块发送EMM_INTRA_ATTACH_REQ消息 */
    ulSendResult = NAS_EMM_SendIntraAttachReq();
    if (NAS_EMM_PLMN_ERR == ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsPlmnSearchMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
        return NAS_LMM_MSG_HANDLED;
    }

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32 NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd
(
    VOS_UINT32              ulMsgId,
    VOS_VOID               *pMsg
)
{
    VOS_UINT32                          ulSendResult;
    EMMC_EMM_SYS_INFO_IND_STRU         *pstMmcSysInfoInd = NAS_EMM_NULL_PTR;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    NAS_MM_TA_STRU                     *pstLastAttmpRegTa = NAS_EMM_NULL_PTR;
    NAS_MM_TA_STRU                      stCurTa;
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    PS_MEM_SET(&stCurTa, 0, sizeof(NAS_MM_TA_STRU));
    (VOS_VOID)ulMsgId;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd is entered.");

    /* 状态匹配检查,若不匹配,退出 */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NO_CELL_AVAILABLE))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 如果当前处于挂起态，则不处理SYS_INFO,直接丢弃*/
    if(NAS_LMM_CUR_LTE_SUSPEND == NAS_EMM_GetCurLteState())
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd:LTE SUSPEND.");
        return NAS_LMM_MSG_HANDLED;
    }

    pstMmcSysInfoInd = (EMMC_EMM_SYS_INFO_IND_STRU *) pMsg;

    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsAttemptToAttachMsgMmcSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    if ((EMMC_EMM_NO_FORBIDDEN != pstMmcSysInfoInd->ulForbiddenInfo) /* TA在禁止的TA列表中 */
        || (EMMC_EMM_CELL_STATUS_ANYCELL == pstMmcSysInfoInd->ulCellStatus))
    {

        /* 将状态转移至MS_DEREG + SS_DEREG_LIMITED_SERVICE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_LIMITED_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        NAS_EMM_SetAttType();

        /*向MMC发送LMM_MMC_ATTACH_CNF或LMM_MMC_ATTACH_IND消息*/
        NAS_EMM_AppSendAttRsltForb(pstMmcSysInfoInd->ulForbiddenInfo);
    }
    else
    {
        /* 获取当前TA和上次尝试注册的TA信息 */
        NAS_EMM_GetCurrentTa(&stCurTa);
        pstLastAttmpRegTa                  = NAS_LMM_GetEmmInfoNetInfoLastAttmpRegTaAddr();

        /* 判断是否需要等定时器超时再发起注册 */
        if (NAS_LMM_MATCH_SUCCESS == NAS_LMM_TaMatch(pstLastAttmpRegTa, &stCurTa))
        {
            NAS_EMM_ATTACH_LOG_NORM("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd: The same as last attempt TA.");

            if(NAS_EMM_YES == NAS_EMM_Attach_IsTimerRunning())
            {
                return NAS_LMM_MSG_HANDLED;
            }
        }
        else
        {
            /*重设attach attempt counter*/
            NAS_EMM_AttResetAttAttempCounter();
        }

        /* 将状态转移至MS_DEREG + SS_DEREG_NORMAL_SERVICE状态 */
        EmmState.enMainState            = EMM_MS_DEREG;
        EmmState.enSubState             = EMM_SS_DEREG_NORMAL_SERVICE;
        EmmState.enStaTId               = TI_NAS_EMM_STATE_NO_TIMER;
        NAS_LMM_StaTransProc(EmmState);

        /* 向ATTACH模块发送EMM_INTRA_ATTACH_REQ消息 */
        ulSendResult = NAS_EMM_SendIntraAttachReq();
        if (NAS_EMM_PLMN_ERR == ulSendResult)
        {
            NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoCellAvailMsgMmcSysInfoInd : Send IntraMsg Failure!!!");
            return NAS_LMM_MSG_HANDLED;
        }
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd(
                    VOS_UINT32                              ulMsgId,
                    const VOS_VOID                         *pMsg )
{
    NAS_EMM_FSM_STATE_STRU                      EmmState;

    (VOS_VOID)(ulMsgId);

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcErrInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /*停止所有定时器*/
    NAS_LMM_StopAllPtlTimer();
    NAS_LMM_StopAllStateTimer();

    /* 释放可能存在的动态内存 */
    NAS_LMM_FreeDynMem();

    /* 写入EMM NV相关信息 */
    NAS_EMM_WriteNvMmInfo();

    /* 写入MMC NV相关信息 */
    /*NAS_EMMC_WriteNvMmInfo();*/

    /*状态机清空，并状态赋初值*/
    NAS_LMM_PUBM_Init();

    /*向ESM发送ID_EMM_ESM_STATUS_IND消息*/
    NAS_EMM_EsmSendStatResult(          EMM_ESM_ATTACH_STATUS_DETACHED);

    /*通知 RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();


    /* 发送内部的AppStartReq */
    /*  V7R1不能自动开机，而是等待MMC的开机  */
    /*    NAS_LMM_SendStubIntraAppStartReq();*/

    /* 通知MMC */
    NAS_EMM_SendMmcErrInd(0xff);


    /* 状态转换 */
    /* 将状态转移至MS_NULL + EMM_SS_NULL_WAIT_APP_START_REQ状态 */
    EmmState.enFsmId                    = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32    NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg )
{
    LRRC_LMM_PAGING_IND_STRU    * pMsgRrcPagingInd = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;

    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd is entered.");

    /*check the current state*/
    if (EMM_MS_NULL == NAS_EMM_CUR_MAIN_STAT)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd: STATE ERROR!!");
        return NAS_LMM_MSG_DISCARD;
    }
    #if 0
    if (NAS_EMM_CONN_IDLE != NAS_EMM_GetConnState())
    {
        /*更新连接状态*/
        NAS_EMM_MrrcChangeRrcStatusToIdle();
    }
    #endif

    /*get RRC_PAGING_IND Msg*/
    pMsgRrcPagingInd = (LRRC_LMM_PAGING_IND_STRU *)pMsg;

    /*check the CN domain:
       LRRC_LNAS_PAGING_TYPE1->CS;
       LRRC_LNAS_PAGING_TYPE2->PS*/
    if(LRRC_LNAS_PAGING_TYPE1 == pMsgRrcPagingInd->enPagingType)
    {
        NAS_EMM_PLMN_CsPagingProc(pMsgRrcPagingInd->enPagingUeId);

        return NAS_LMM_MSG_HANDLED;
    }

    /*Paging with IMSI*/
    if(LRRC_LNAS_IMSI_LTE == pMsgRrcPagingInd->enPagingUeId)
    {
        #if (FEATURE_PTM == FEATURE_ON)
        NAS_EMM_PagingErrRecord(EMM_OM_ERRLOG_PAGING_PS_IMSI);
        #endif
        NAS_EMM_PLMN_ImsiPagingProc();
    }
    /*Paging with S-TMSI*/
    else if(LRRC_LNAS_S_TMSI_LTE == pMsgRrcPagingInd->enPagingUeId)
    {
        NAS_EMM_PLMN_STmsiPagingProc();
    }
    /*Paging with Other UeId*/
    else
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsNotNullSsAnyStateMsgRrcPagingInd:Other UeId not support");
    }

    return NAS_LMM_MSG_HANDLED;
}
VOS_UINT32    NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd(
                    VOS_UINT32                              ulMsgId,
                    VOS_VOID                                *pMsg)
{
    /*VOS_UINT32                          ulSendResult;*/
    NAS_EMM_FSM_STATE_STRU              EmmState;

    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /* 打印进入该函数，INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (EMM_MS_NULL == EmmState.enMainState)
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }
    NAS_LMM_EMM_PLMN_LOG_INFO("NAS_EMM_MsDeregSsNoImsiMsgMmcPlmnInd: Set NAS_EMM_PLMN_SRCH_NO_IMSI_TRUE!!!");

    /* 判断若为挂起态，则通知ESM和ERABM激活，但不获取GU的安全参数 */
    NAS_EMM_NoImsiSuspendMsgMmcPlmnReq();

    return NAS_LMM_MSG_HANDLED;
}


VOS_UINT32  NAS_EMM_MsNullSsAnyStateMsgRrcErrInd(
    VOS_UINT32                                              ulMsgId,
    VOS_VOID                                                *pMsg)

{
    (VOS_VOID)(ulMsgId);
    (VOS_VOID)(pMsg);

    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsAnyStateMsgRrcErrInd: enter.");


    return NAS_LMM_MSG_DISCARD;

}



VOS_VOID  NAS_EMM_ProcRrcStopRslt( VOS_VOID )
{
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(EmmState);

    /*关机初始化 PUBM*/
    NAS_LMM_PUBM_Init();

    /* 将状态转移至MS_NULL + SS_WAIT_APP_START_REQ状态 */
    EmmState.enFsmId                = NAS_LMM_PARALLEL_FSM_EMM;
    EmmState.enMainState            = EMM_MS_NULL;
    EmmState.enSubState             = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enFsmId                = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return;
}

#if 0

VOS_UINT32  NAS_EMM_MsNullSsWaitRrcStartCnfProcMsgTimerExp(VOS_UINT32 ulErrCause)
{
    VOS_UINT32                          ulSendOuterMsgResult;
    VOS_UINT32                          ulSendInnerMsgResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;

    NAS_EMM_PLMN_INIT_EMMSTATE(         EmmState);

    /*停止所有EMM状态定时器*/
    NAS_LMM_StopAllEmmStateTimer();

    /*停止所有协议定时器*/
    NAS_LMM_StopAllPtlTimer();

    /* 向MMC发送MMC_EMM_STOP_REQ消息 */
    ulSendInnerMsgResult = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR == ulSendInnerMsgResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_PLMN_SendMmcStopReq : Send IntraMsg Failure!!!");
    }

    if (NAS_EMM_PLMN_OK == NAS_EMM_CheckAppMsgPara(ID_MMC_LMM_START_REQ))
    {
        /* 向APP发送APP_MM_START_CNF消息（回复开机失败，等待RRC启动超时） */
        ulSendOuterMsgResult= NAS_EMM_PLMN_SendAppStartCnf(
                                        ulErrCause, NAS_EMM_GetAppMsgOpId());

        if (NAS_EMM_PLMN_OK != ulSendOuterMsgResult)
        {
            /* 打印函数，ERROR_LEVEL */
            NAS_LMM_EMM_PLMN_LOG1_ERR("NAS_EMM_PLMN_SendAppStartCnf : Send Msg Failure!!! ulSendOuterMsgResult=",
                                        ulSendOuterMsgResult);
        }
    }

    /*清空APP参数*/
    NAS_EMM_ClearAppMsgPara();


    /* 将状态转移至MS_NULL + SS_NULL_WAIT_APP_START_REQ状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_APP_START_REQ;
    EmmState.enStaTId                   = TI_NAS_EMM_STATE_NO_TIMER;
    NAS_LMM_StaTransProc(EmmState);

    return(NAS_LMM_MSG_HANDLED);
}
#endif


VOS_UINT32  NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd(VOS_VOID)
{
    VOS_UINT32                          ulSendResult;
    NAS_EMM_FSM_STATE_STRU              EmmState;
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */
    VOS_UINT8                           ucImsiRefreshFlag;
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */

    NAS_EMM_PLMN_INIT_EMMSTATE(         EmmState);

    /* 通知ESM清除资源 */
    NAS_EMM_PLMN_SendEmmEsmStautsInd(   EMM_ESM_ATTACH_STATUS_DETACHED);

    /*通知 RABM:RRC_CONN_REL_IND*/
    NAS_LMM_SendRabmRrcConRelInd();

    /* 通知ETC:REL_IND */
    NAS_EMM_TcSendRelInd();

    /*停止定时器*/
    NAS_LMM_StopStateTimer(              TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    /* 向MMC发送MMC_EMM_STOP_REQ消息 */
    ulSendResult                        = NAS_EMM_PLMN_SendMmcStopReq();
    if (NAS_EMM_PLMN_ERR                == ulSendResult)
    {
        /* 打印函数，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcRelInd : Send Msg Failure!!!");
        return(                         NAS_LMM_MSG_HANDLED);
    }

    /* 如果current安全上下文为mapped 且non-current native上下文存在，
       将后者设置为current，同时删除current mapped，以便关机时写入卡中
       依据:      24301协议     5.5.2.2.1*/
    NAS_EMM_SecuDeregClrSecuCntxt();

    /* 写入EMM NV相关信息 */
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,Begin */
    ucImsiRefreshFlag = NAS_MML_GetImsiRefreshStatus();
    NAS_LMM_SndOmImsiRefreshStatus(ucImsiRefreshFlag);
    /* IMSI REFRESH 情况下不写卡 */
    if(VOS_FALSE == ucImsiRefreshFlag)
    {
        NAS_EMM_WriteNvMmInfo();
    }
    /* Add by y00307272 for IMSI REFRESH PORTECT,2015-11-18,End */

    /* 启动TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER */
    NAS_LMM_StartStateTimer(             TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER);

    /* 将状态转移至MS_NULL + SS_NULL_WAIT_MMC_STOP_CNF状态 */
    EmmState.enMainState                = EMM_MS_NULL;
    EmmState.enSubState                 = EMM_SS_NULL_WAIT_MMC_STOP_CNF;
    EmmState.enStaTId                   = TI_NAS_EMM_WAIT_MMC_STOP_CNF_TIMER;
    NAS_LMM_StaTransProc(                EmmState);

    return(                             NAS_LMM_MSG_HANDLED);
}



VOS_UINT32  NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd(
    VOS_UINT32                                              ulMsgId,
    VOS_VOID                                                *pMsg )
{
    VOS_UINT32                          ulRet;

    (VOS_VOID)(                         ulMsgId);
    (VOS_VOID)(                         pMsg);

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM(           "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR                    == pMsg)
    {
        /* 打印异常，ERROR_LEVEL */
        NAS_LMM_EMM_PLMN_LOG_ERR(        "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd: MSG PARAM NULL POINTER ERROR!!!");
        return(                         NAS_LMM_ERR_CODE_PARA_INVALID);
    }

    /* 状态匹配检查,若不匹配,退出 */
    if (NAS_EMM_PLMN_CHK_STAT_INVALID(  EMM_MS_NULL, EMM_SS_NULL_WAIT_SWITCH_OFF))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN(       "NAS_EMM_MsNullSsWaitSwitchOffMsgRrcErrInd: STATE ERROR!!!");
        return(                         NAS_LMM_MSG_DISCARD);
    }

    /* 停止TI_NAS_EMM_PTL_SWITCH_OFF_TIMER */
    NAS_LMM_StopPtlTimer(                TI_NAS_EMM_PTL_SWITCH_OFF_TIMER);

    ulRet                               = NAS_EMM_MsNullSsWaitSwitchOffProcMsgRrcRelInd();

    return(                             ulRet);
}
VOS_UINT32 NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd(
                                    VOS_UINT32              ulMsgId,
                                    VOS_VOID               *pMsg )
{
    NAS_EMM_ESM_MSG_BUFF_STRU          *pstEsmMsg = NAS_EMM_NULL_PTR;

    (VOS_VOID)ulMsgId;
    (VOS_VOID)pMsg;

    /* 打印进入该函数， INFO_LEVEL */
    NAS_LMM_EMM_PLMN_LOG_NORM("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd is entered.");

    /* 函数输入指针参数检查 */
    if (VOS_NULL_PTR == pMsg)
    {
        NAS_LMM_EMM_PLMN_LOG_ERR("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: MSG PARAM NULL POINTER ERROR!!!");
        return NAS_LMM_ERR_CODE_PARA_INVALID;
    }

    /* 状态匹配检查,若不匹配,退出 */
    if(NAS_EMM_PLMN_CHK_STAT_INVALID(EMM_MS_DEREG, EMM_SS_DEREG_NO_IMSI))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: STATE ERROR!!!");
        return NAS_LMM_MSG_DISCARD;
    }

    /* 如果ATTACH的触发类型是紧急PDN建立引起的普通注册，且缓存的紧急PDN建立请求
       消息有效，则发起紧急注册 */
    pstEsmMsg = (NAS_EMM_ESM_MSG_BUFF_STRU*)(VOS_VOID*)NAS_LMM_GetEmmInfoEsmBuffAddr();
    if ((EMM_ATTACH_CAUSE_ESM_ATTACH_FOR_INIT_EMC_BERER == NAS_EMM_GLO_AD_GetAttCau())
        && (NAS_EMM_NULL_PTR != pstEsmMsg))
    {
        NAS_LMM_EMM_PLMN_LOG_WARN("NAS_EMM_MsDrgSsNoImsiMsgSysInfoInd: case ESM_ATTACH_FOR_INIT_EMC_BERER");

        /*保存ESM消息*/
        NAS_LMM_MEM_CPY(        (VOS_VOID*)NAS_EMM_GLO_AD_GetEsmMsgAddr(),
                               &(pstEsmMsg->stEsmMsg),
                                (pstEsmMsg->stEsmMsg.ulEsmMsgSize)+4);

        NAS_EMM_StartEmergencyAttach();

        return NAS_LMM_MSG_HANDLED;
    }

    NAS_EMM_AppSendAttRstDefaultReqType(MMC_LMM_ATT_RSLT_FAILURE);

    return NAS_LMM_MSG_HANDLED;
}
/*lint +e961*/
/*lint +e960*/
#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

