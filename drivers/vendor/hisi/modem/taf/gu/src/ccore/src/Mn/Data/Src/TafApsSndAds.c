


/*****************************************************************************
  1 头文件包含
*****************************************************************************/



#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif
#endif


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_TAF_APS_SNDADS_C

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "TafApsSndAds.h"

/******************************************************************************
   2 外部函数变量声明
******************************************************************************/


/******************************************************************************
   3 私有定义
******************************************************************************/


/******************************************************************************
   4 全局变量定义
*****************************************************************************/


/******************************************************************************
   5 函数实现
******************************************************************************/



ADS_QCI_TYPE_ENUM_UINT8 TAF_APS_QosParaTransferToQci(
    TAF_UMTS_QOS_STRU                   *pQos
)
{
    ADS_QCI_TYPE_ENUM_UINT8             enQci;

    /* 转换规则参见: 23.401
       Annex E (normative):
        Mapping between EPS and Release 99 QoS parameters
        QCI    Traffic_Class  Traffic_Handling_Priority Signalling_Indication  Src_Stats_Descriptor
         1     Conversational          NA                       NA                  SPEECH
         2     Conversational          NA                       NA                    NA
         3     Conversational          NA                       NA                   Unknown
         4     Streaming               NA                       NA                   Unknown
         5     Interactive              1                       YES                  Unknown
         6     Interactive              1                       NO                    NA
         7     Interactive              2                       NO                    NA
         8     Interactive              3                       NO                    NA
         9     Background              NA                       NA                    NA

      NOTE°1: When QCI 2 is mapped to Release 99 QoS parameter values, the Transfer Delay parameter
               is set to 150 ms. When Release 99 QoS parameter values are mapped to a QCI,
               QCI 2 is used for conversational/unknown if the Transfer Delay parameter is greater
               or equal to 150 ms.
      NOTE°2: When QCI 3 is mapped to Release 99 QoS parameter values, the Transfer Delay parameter
               is set to 80 ms as the lowest possible value, according to TS 23.107 [59]. When Release
               99 QoS parameter values are mapped to a QCI, QCI 3 is used for conversational/unknown if
               the Transfer Delay parameter is lower than 150 ms.
      NOTE  3: When QCI 4 is mapped to Release 99 QoS parameter values, it is mapped to Streaming/Unknown.
               When Release 99 QoS parameter values are mapped to a QCI, Streaming/Unknown and Streaming/Speech
               are both mapped to QCI 4.
    */
    /* 根据QOS trafficClass参数获取QCI */
    if (TAF_APS_APP_QOS_TRAFFIC_CLASS_CONVERSATIONAL == pQos->ucTrafficClass)
    {
        if (TAF_APS_APP_QOS_SRC_STATS_DESCRIPTOR_SPEECH == pQos->ucSrcStatisticsDescriptor)
        {
            enQci       = ADS_QCI_TYPE_QCI1_GBR;
        }
        else if (pQos->usTransDelay >= TAF_APS_APP_QOS_TRANSFER_DELAY_150MS)
        {
            enQci       = ADS_QCI_TYPE_QCI2_GBR;
        }
        else
        {
            enQci       = ADS_QCI_TYPE_QCI3_GBR;
        }
    }
    else if (TAF_APS_APP_QOS_TRAFFIC_CLASS_STREAMING == pQos->ucTrafficClass)
    {
        enQci       = ADS_QCI_TYPE_QCI4_GBR;
    }
    else if (TAF_APS_APP_QOS_TRAFFIC_CLASS_INTERACTIVE == pQos->ucTrafficClass)
    {
        if (TAF_APS_APP_QOS_TRAFFIC_HANDLING_PRIORITY_1 == pQos->ucTraffHandlePrior)
        {
            if (TAF_APS_APP_QOS_SIGNAL_IND_OPTIMISE == pQos->ucSignallingIndication)
            {
                enQci       = ADS_QCI_TYPE_QCI5_NONGBR;
            }
            else
            {
                enQci       = ADS_QCI_TYPE_QCI6_NONGBR;
            }
        }
        else if (TAF_APS_APP_QOS_TRAFFIC_HANDLING_PRIORITY_2 == pQos->ucTraffHandlePrior)
        {
            enQci       = ADS_QCI_TYPE_QCI7_NONGBR;
        }
        else
        {
            enQci       = ADS_QCI_TYPE_QCI8_NONGBR;
        }
    }
    else
    {
        enQci       = ADS_QCI_TYPE_QCI9_NONGBR;
    }

    return enQci;
}
VOS_UINT32 TAF_APS_SendAdsPdpStatusInd(
    VOS_UINT32                          ulPid,
    ADS_PDP_STATUS_IND_STRU            *pstAdsInd
)
{
    ADS_PDP_STATUS_IND_STRU            *pstMsg;

    /* 申请ADS_PDP_STATUS_IND_STRU消息 */
    pstMsg = (ADS_PDP_STATUS_IND_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                            WUEPS_PID_TAF,
                            sizeof(ADS_PDP_STATUS_IND_STRU));

    if (VOS_NULL_PTR == pstMsg)
    {
        APS_WARN_LOG("Taf_APS_SendAdsPdpStatusInd: alloc msg fail!");
        return VOS_ERR;
    }

    /* 初始化消息 */
    PS_MEM_SET((VOS_CHAR*)pstMsg + VOS_MSG_HEAD_LENGTH,
               0x00,
               sizeof(ADS_PDP_STATUS_IND_STRU) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息头 */
    pstMsg->ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->ulReceiverPid   = ulPid;
    pstMsg->enMsgId         = ID_APS_ADS_PDP_STATUS_IND;

    /* 填写消息体 */
    pstMsg->enModemId       = pstAdsInd->enModemId;
    pstMsg->ucRabId         = pstAdsInd->ucRabId;
    pstMsg->enPdpStatus     = pstAdsInd->enPdpStatus;
    pstMsg->enPdpType       = pstAdsInd->enPdpType;
    pstMsg->enQciType       = pstAdsInd->enQciType;

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_TAF, pstMsg))
    {
        APS_WARN_LOG("TAF_APS_SendAdsPdpStatusInd: Send msg fail!");
        return VOS_ERR;
    }

    return VOS_OK;
}


VOS_VOID TAF_APS_NotifyAdsWhenPdpModify(
    TAF_PS_CALL_PDP_MODIFY_CNF_STRU    *pstEvent
)
{
    ADS_PDP_STATUS_IND_STRU             stAdsInd;

    PS_MEM_SET(&stAdsInd, 0x00, sizeof(ADS_PDP_STATUS_IND_STRU));
    stAdsInd.enModemId = MODEM_ID_0;

#if 0
#if (FEATURE_ON == FEATURE_IMS)
    /* 当module id为IMSA或者是专有承载时，不给ADS发送PDP状态改变通知消息 */
    if (PS_PID_IMSA == pstEvent->stCtrl.ulModuleId)
    {
        APS_WARN_LOG("TAF_APS_NotifyAdsWhenPdpModify:module id is imsa");
        return;
    }
#endif
#endif

    stAdsInd.enModemId = VOS_GetModemIDFromPid(WUEPS_PID_TAF);

    if (MODEM_ID_BUTT == stAdsInd.enModemId)
    {
        APS_ERR_LOG("TAF_APS_NotifyAdsWhenPdpModify:Get Modem Id fail");
        return;
    }

    /* 填写参数 */
    stAdsInd.enPdpStatus    = ADS_PDP_STATUS_MODIFY;
    stAdsInd.enPdpType      = TAF_PDP_TYPE_BUTT;
    stAdsInd.ucRabId        = pstEvent->ucRabId;
    stAdsInd.enQciType      = TAF_APS_QosParaTransferToQci(&pstEvent->stUmtsQos);

    /* 向ADS_DL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_DL,
                           &stAdsInd);

    /* 向ADS_UL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_UL,
                           &stAdsInd);

    return;
}


VOS_VOID TAF_APS_NotifyAdsWhenPdpDeactivated(
    TAF_PS_CALL_PDP_DEACTIVATE_CNF_STRU    *pstEvent
)
{
    ADS_PDP_STATUS_IND_STRU             stAdsInd;

    PS_MEM_SET(&stAdsInd, 0x00, sizeof(ADS_PDP_STATUS_IND_STRU));
    stAdsInd.enModemId = MODEM_ID_0;

#if 0
#if (FEATURE_ON == FEATURE_IMS)
    /* 当module id为IMSA或者是专有承载时，不给ADS发送PDP状态改变通知消息 */
    if (PS_PID_IMSA == pstEvent->stCtrl.ulModuleId)
    {
        APS_WARN_LOG("TAF_APS_NotifyAdsWhenPdpDeactivated:module id is imsa");
        return;
    }
#endif
#endif

    stAdsInd.enModemId = VOS_GetModemIDFromPid(WUEPS_PID_TAF);

    if (MODEM_ID_BUTT == stAdsInd.enModemId)
    {
        APS_ERR_LOG("TAF_APS_NotifyAdsWhenPdpDeactivated:Get Modem Id fail");
        return;
    }

    /* 填写参数 */
    stAdsInd.enPdpStatus    = ADS_PDP_STATUS_DEACT;
    stAdsInd.enPdpType      = pstEvent->enPdpType;
    stAdsInd.ucRabId        = pstEvent->ucRabId;
    stAdsInd.enQciType      = ADS_QCI_TYPE_BUTT;

    /* 向ADS_DL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_DL,
                           &stAdsInd);

    /* 向ADS_UL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_UL,
                           &stAdsInd);

    return;
}


VOS_VOID TAF_APS_NotifyAdsWhenPdpAvtivated(
    TAF_PS_CALL_PDP_ACTIVATE_CNF_STRU  *pstEvent
)
{
    ADS_PDP_STATUS_IND_STRU             stAdsInd;

    PS_MEM_SET(&stAdsInd, 0x00, sizeof(ADS_PDP_STATUS_IND_STRU));
    stAdsInd.enModemId = MODEM_ID_0;

#if 0
#if (FEATURE_ON == FEATURE_IMS)
    /* 当module id为IMSA或者是专有承载时，不给ADS发送PDP状态改变通知消息 */
    if (PS_PID_IMSA == pstEvent->stCtrl.ulModuleId)
    {
        APS_WARN_LOG("TAF_APS_NotifyAdsWhenPdpAvtivated:module id is imsa");
        return;
    }
#endif
#endif

    stAdsInd.enModemId = VOS_GetModemIDFromPid(WUEPS_PID_TAF);

    if (MODEM_ID_BUTT == stAdsInd.enModemId)
    {
        APS_ERR_LOG("TAF_APS_NotifyAdsWhenPdpAvtivated:Get Modem Id fail");
        return;
    }

    /* 填写参数 */
    stAdsInd.enPdpStatus    = ADS_PDP_STATUS_ACT;
    stAdsInd.enPdpType      = pstEvent->stPdpAddr.enPdpType;
    stAdsInd.ucRabId        = pstEvent->ucRabId;
    stAdsInd.enQciType      = TAF_APS_QosParaTransferToQci(&pstEvent->stUmtsQos);

    /* 向ADS_DL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_DL,
                           &stAdsInd);

    /* 向ADS_UL发送PDP修改指示 */
    TAF_APS_SendAdsPdpStatusInd(ACPU_PID_ADS_UL,
                           &stAdsInd);

    return;
}


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


