
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "pslog.h"
#include "NasComm.h"
#include "om.h"
#include "NasMmcSndOm.h"
#include "MM_Ext.h"
#include "GmmExt.h"
#include "NasMmcPlmnSelectionStrategy.h"
#include "NasMmcCtx.h"
#include "NasMmcProcUsim.h"

#include  "NasMmlCtx.h"

#include "NasMmcFsmMainTbl.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_NAS_MMC_SND_OM_C

/*****************************************************************************
  2 全局变量
*****************************************************************************/
/*****************************************************************************
  3 宏定义
*****************************************************************************/


/*****************************************************************************
 函 数 名  : NAS_MMC_LogMmcState
 功能描述  : 打印出MMC的内部状态
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年1月13日
   作    者   : zhoujun /40661
   修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID  NAS_MMC_LogMmcFsmInfo( VOS_VOID )
{
    NAS_MMC_LOG_FSM_INFO_STRU           *pstMsg = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_FSM_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                             sizeof(NAS_MMC_LOG_FSM_INFO_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogMmcFsmInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_FSM_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_FSM_INFO_IND;
    pstMsg->enFsmId                     = NAS_MMC_GetCurrFsmId();
    pstMsg->ulTopState                  = NAS_MMC_GetFsmTopState();

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}

#if (FEATURE_LTE == FEATURE_ON)

VOS_VOID NAS_MMC_LogGutiInfo(
    NAS_LMM_GUTI_STRU                  *pstGuti
)
{
    NAS_MMC_LOG_GUTI_INFO_STRU         *pstGutiMsg = VOS_NULL_PTR;

    pstGutiMsg = (NAS_MMC_LOG_GUTI_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                             sizeof(NAS_MMC_LOG_GUTI_INFO_STRU));
    if ( VOS_NULL_PTR == pstGutiMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_GMM, "NAS_MMC_LogGutiInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    pstGutiMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstGutiMsg->stMsgHeader.ulSenderPid     = PS_PID_MM;
    pstGutiMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstGutiMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_GUTI_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstGutiMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_GUTI_INFO_IND;

    PS_MEM_CPY(&(pstGutiMsg->stGutiMsg), pstGuti, sizeof(NAS_LMM_GUTI_STRU));

    OM_TraceMsgHook(pstGutiMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstGutiMsg);

    return;
}
#endif


/*****************************************************************************
 函 数 名  : NAS_MMC_LogBufferQueueMsg
 功能描述  : 导出缓存的消息
 输入参数  : ulFullFlg:缓存是否已经满的标志位
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2011年6月7日
    作    者   : zhoujun /40661
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID  NAS_MMC_LogBufferQueueMsg(
    VOS_UINT32                          ulFullFlg
)
{
    NAS_MMC_LOG_BUffER_MSG_INFO_STRU    *pstBufferMsg = VOS_NULL_PTR;
    NAS_MMC_MSG_QUEUE_STRU              *pstMsgQueue  = VOS_NULL_PTR;

    pstBufferMsg = (NAS_MMC_LOG_BUffER_MSG_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                             sizeof(NAS_MMC_LOG_BUffER_MSG_INFO_STRU));
    if ( VOS_NULL_PTR == pstBufferMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_GMM, "NAS_MMC_LogBufferQueueMsg:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsgQueue                               = NAS_MMC_GetCachMsgBufferAddr();
    pstBufferMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstBufferMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstBufferMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstBufferMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_BUffER_MSG_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstBufferMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_BUFFER_MSG_INFO_IND;
    pstBufferMsg->ulFullFlg                   = ulFullFlg;
    PS_MEM_CPY(&(pstBufferMsg->stMsgQueue), pstMsgQueue, sizeof(pstBufferMsg->stMsgQueue));

    OM_TraceMsgHook(pstBufferMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstBufferMsg);

    return;
}



VOS_VOID NAS_MMC_SndOutsideFixedContextData( VOS_VOID )
{
    NAS_MMC_FIXED_CONTEXT_MSG_STRU                         *pstFixedCtx;
    NAS_MML_CTX_STRU                                       *pstMmlCtx;
    NAS_MMC_CONTEXT_STRU                                   *pstMmcCtx;
    VOS_UINT8                                              *pucCompressBuf;
    NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU                *pstPcreplayCompressCnt;
    VOS_UINT32                                              ulCompressDataLen;
    VOS_UINT32                                              ulCount;
    VOS_UINT8                                              *pucEndBuf;
    VOS_UINT32                                              ulItemCount;
    VOS_UINT8                                              *pucDest;
    VOS_UINT8                                              *pucSrc;
    VOS_UINT32                                              ulCmLen;

    ulCount = sizeof(NAS_MMC_FIXED_CONTEXT_MSG_STRU);
    ulItemCount = 0;

    pstFixedCtx = (NAS_MMC_FIXED_CONTEXT_MSG_STRU *)PS_ALLOC_MSG
                         (WUEPS_PID_MMC, sizeof(NAS_MMC_FIXED_CONTEXT_MSG_STRU));

    if ( VOS_NULL_PTR == pstFixedCtx )
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOutsideFixedContextData:ERROR: Alloc Memory Fail.");
        return;
    }

    ulCompressDataLen  = NAS_MML_PC_RECUR_COMPRESS_DATA_LEN;
    pucCompressBuf     = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_MMC, ulCompressDataLen);

    if (VOS_NULL_PTR == pucCompressBuf)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOutsideFixedContextData: Alloc Fail.");
        PS_FREE_MSG(WUEPS_PID_MMC, pstFixedCtx);
        return;
    }

    pstPcreplayCompressCnt = (NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU *)pucCompressBuf;

    /* 获取buf结尾地址，用于计算剩余空间*/
    pucEndBuf = (VOS_UINT8 *)(pucCompressBuf + ulCompressDataLen);

    /* 压缩 fix data */
    pucDest  = pstPcreplayCompressCnt->aucData;

    /* 原始数据大小 */
    PS_MEM_CPY(pucDest, &ulCount, sizeof(VOS_UINT32));
    pucDest += sizeof(VOS_UINT32);

    pstMmlCtx  =  NAS_MML_GetMmlCtx();
    pstMmcCtx  =  NAS_MMC_GetMmcCtxAddr();

    /* 保存IMEI的内容 */
    PS_MEM_CPY(pstFixedCtx->aucImeisv,
               pstMmlCtx->stMsCfgInfo.stMsCapability.aucImeisv,
               NAS_MML_MAX_IMEISV_LEN);

    /* 保存搜网模式 */
    pstFixedCtx->enSelectionMode = pstMmcCtx->stPlmnSearchCtrl.enSelectionMode;

    pstFixedCtx->ucPsAutoAttachFlg = pstMmlCtx->stMsCfgInfo.stMsSysCfgInfo.ucPsAutoAttachFlg;

    /* 保存等效PLMN */
    PS_MEM_CPY(&(pstFixedCtx->stEquPlmnInfo),
               &(pstMmlCtx->stNetworkInfo.stEquPlmnInfo),
               sizeof(NAS_MML_EQUPLMN_INFO_STRU));

    /* 保存等效RPLMN相关定制 */
    PS_MEM_CPY(&(pstFixedCtx->stRplmnCfg),
               &(pstMmlCtx->stMsCfgInfo.stCustomCfg.stRplmnCfg),
               sizeof(NAS_MML_RPLMN_CFG_INFO_STRU));

    /* 保存网络能力 */
    pstFixedCtx->stMsNetworkCapability.ucNetworkCapabilityLen = pstMmlCtx->stMsCfgInfo.stMsCapability.stMsNetworkCapability.ucNetworkCapabilityLen;

    if (pstFixedCtx->stMsNetworkCapability.ucNetworkCapabilityLen > NAS_MML_MAX_NETWORK_CAPABILITY_LEN)
    {
        pstFixedCtx->stMsNetworkCapability.ucNetworkCapabilityLen  = NAS_MML_MAX_NETWORK_CAPABILITY_LEN;
    }
    PS_MEM_CPY(pstFixedCtx->stMsNetworkCapability.aucNetworkCapability,
               pstMmlCtx->stMsCfgInfo.stMsCapability.stMsNetworkCapability.aucNetworkCapability,
               NAS_MML_MAX_NETWORK_CAPABILITY_LEN);


    /* 保存BG搜网相关信息 */
    PS_MEM_CPY(&(pstFixedCtx->stBGSrchCfg),
               &(pstMmlCtx->stMsCfgInfo.stCustomCfg.stBgSearchCfg),
               sizeof(NAS_MML_BG_SEARCH_CFG_INFO_STRU));

    /* NV项的恢复,恢复快速开机 */
    pstFixedCtx->ulQuickStartFlag = NAS_MML_IsQuickStartFlg();


    /* 压缩NAS_MMC_FIXED_CONTEXT_MSG_STRU */
    pucSrc      = (VOS_UINT8 *)pstFixedCtx;
    ulItemCount = sizeof(NAS_MMC_FIXED_CONTEXT_MSG_STRU);
    ulCmLen     = (VOS_UINT32)(pucEndBuf - pucDest + 1);
    if( VOS_FALSE == NAS_MML_CompressData(pucDest, &ulCmLen , pucSrc , ulItemCount) )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);
        PS_FREE_MSG(WUEPS_PID_MMC, pstFixedCtx);
        return;
    }
    pucDest          += (ulCmLen);
    ulCompressDataLen = (VOS_UINT32)(pucDest - (VOS_UINT8 *)pstPcreplayCompressCnt);

    pstPcreplayCompressCnt->ulReceiverPid      = WUEPS_PID_MMC;
    pstPcreplayCompressCnt->ulSenderPid        = WUEPS_PID_MMC;
    pstPcreplayCompressCnt->ulSenderCpuId      = VOS_LOCAL_CPUID;
    pstPcreplayCompressCnt->ulReceiverCpuId    = VOS_LOCAL_CPUID;
    pstPcreplayCompressCnt->ulLength           = ulCompressDataLen;
    pstPcreplayCompressCnt->ulMsgName          = MMCOM_FIXED_PART_CONTEXT;
    OM_TraceMsgHook(pstPcreplayCompressCnt);

    PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);
    PS_FREE_MSG(WUEPS_PID_MMC, pstFixedCtx);

}
VOS_VOID NAS_MMC_SndOutsideContextData( VOS_VOID )
{
    VOS_UINT8                                              *pucCompressBuf;
    NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU                *pstPcreplayCompressCtx;
    VOS_UINT32                                              ulCompressDataLen;
    VOS_UINT32                                              ulCount;
    VOS_UINT8                                              *pucEndBuf;
    VOS_UINT32                                              ulItemCount;
    VOS_UINT8                                              *pucDest;
    VOS_UINT8                                              *pucSrc;
    VOS_UINT32                                              ulCmLen;

    ulCount     = sizeof(NAS_MML_CTX_STRU)
                + sizeof(MM_SUB_LAYER_SHARE_STRU)
                + sizeof(NAS_MMC_CONTEXT_STRU)
                + sizeof(TAF_PLMN_ID_STRU);  /* 需要备份 g_stSetRplmn，用于PC回放 */

    ulItemCount = 0;

    ulCompressDataLen  = NAS_MML_PC_RECUR_COMPRESS_DATA_LEN;
    pucCompressBuf     = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_MMC, ulCompressDataLen);

    if(VOS_NULL_PTR == pucCompressBuf)
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC,"NAS_MMC_SndOutsideContextData: Alloc Fail.");
        return;
    }

    pstPcreplayCompressCtx = (NAS_MML_PC_REPLAY_COMPRESS_CONTEXT_STRU *)pucCompressBuf;

    /* 获取buf结尾地址，用于计算剩余空间*/
    pucEndBuf = pucCompressBuf + ulCompressDataLen;

    /* 压缩 fix data */
    pucDest  = pstPcreplayCompressCtx->aucData;

    /* 原始数据大小 */
    PS_MEM_CPY(pucDest, &ulCount, sizeof(VOS_UINT32));
    pucDest += sizeof(VOS_UINT32);

    /* 压缩MML context g_stNasMmlCtx*/
    pucSrc      = (VOS_UINT8 *)NAS_MML_GetMmlCtx();
    ulItemCount = sizeof(NAS_MML_CTX_STRU);
    ulCmLen     = (VOS_UINT32)(pucEndBuf - pucDest + 1);
    if( VOS_FALSE == NAS_MML_CompressData(pucDest, &ulCmLen , pucSrc , ulItemCount) )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);
        return;
    }
    pucDest += (ulCmLen);

    /* 压缩g_MmSubLyrShare */
    pucSrc      = (VOS_UINT8 *)&g_MmSubLyrShare;
    ulItemCount = sizeof(MM_SUB_LAYER_SHARE_STRU);
    ulCmLen     = (VOS_UINT32)(pucEndBuf - pucDest + 1);
    if( VOS_FALSE == NAS_MML_CompressData(pucDest, &ulCmLen, pucSrc, ulItemCount) )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);
        return;
    }
    pucDest += (ulCmLen);

    /* 压缩MMC context g_stNasMmcCtx */
    pucSrc      = (VOS_UINT8 *)NAS_MMC_GetMmcCtxAddr();
    ulItemCount = sizeof(NAS_MMC_CONTEXT_STRU);
    ulCmLen     = (VOS_UINT32)(pucEndBuf - pucDest + 1);
    if( VOS_FALSE == NAS_MML_CompressData(pucDest, &ulCmLen, pucSrc, ulItemCount) )
    {
        PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);
        return;
    }
    pucDest += (ulCmLen);

#if (FEATURE_ON == FEATURE_LTE)
#ifdef WIN32
	/* PC工程需要备份g_stSetRplmn用于回放 */
    PS_MEM_CPY(pucDest,&g_stSetRplmn,sizeof(TAF_PLMN_ID_STRU));
#endif
#endif
    pucDest += sizeof(TAF_PLMN_ID_STRU);

    ulCompressDataLen = (VOS_UINT32)(pucDest - (VOS_UINT8 *)pstPcreplayCompressCtx);

    pstPcreplayCompressCtx->ulReceiverPid      = WUEPS_PID_MMC;
    pstPcreplayCompressCtx->ulSenderPid        = WUEPS_PID_MMC;
    pstPcreplayCompressCtx->ulSenderCpuId      = VOS_LOCAL_CPUID;
    pstPcreplayCompressCtx->ulReceiverCpuId    = VOS_LOCAL_CPUID;
    pstPcreplayCompressCtx->ulLength           = ulCompressDataLen;
    pstPcreplayCompressCtx->ulMsgName          = MMCOM_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY;
    OM_TraceMsgHook(pstPcreplayCompressCtx);


    PS_MEM_FREE(WUEPS_PID_MMC, pucCompressBuf);

}


VOS_VOID NAS_MMC_SndOmOtaCnf(
    VOS_UINT32                          ulErrCode
)
{
    VOS_UINT16                               usNasOtaCnfLen;
    NAS_OM_OTA_CNF_STRUCT                   *pstNasOtaCnf     = VOS_NULL_PTR;

    MODEM_ID_ENUM_UINT16                     enModemId;


    usNasOtaCnfLen  = sizeof(NAS_OM_OTA_CNF_STRUCT);

    pstNasOtaCnf = (NAS_OM_OTA_CNF_STRUCT *)PS_MEM_ALLOC(WUEPS_PID_MMC, usNasOtaCnfLen);

    if ( VOS_NULL_PTR == pstNasOtaCnf )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_RcvOmOtaReq:WARNING: Alloc Memory Fail.");
        return;
    }

    /* 填充 到 OM的透明消息 */
    PS_MEM_SET(pstNasOtaCnf, 0, usNasOtaCnfLen);

    enModemId   =   VOS_GetModemIDFromPid(WUEPS_PID_MMC);
    pstNasOtaCnf->ucFuncType        = ((enModemId << 6) & NAS_OM_FUNCTION_TYPE_MODEM_MASK) \
                                    | (OM_AIR_FUNC & NAS_OM_FUNCTION_TYPE_VALUE_MASK);

    /* 减去消息头长度 */
    pstNasOtaCnf->usLength          = usNasOtaCnfLen - 4;
    OM_AddSNTime( &(pstNasOtaCnf->ulSn), &(pstNasOtaCnf->ulTimeStamp) );

    pstNasOtaCnf->usPrimId          = NAS_OM_OTA_CNF;
    pstNasOtaCnf->ulErrCode         = ulErrCode;

    OM_SendData((OM_RSP_PACKET_STRU *)pstNasOtaCnf, usNasOtaCnfLen);
    PS_MEM_FREE(WUEPS_PID_MMC, pstNasOtaCnf );


}


VOS_VOID NAS_MMC_FillNasMmImsiInfo(
    MS_ID_STRUCT                       *pstMsIdMsg
)
{
    VOS_UINT8                           i = 0;
    VOS_UINT8                           j = 0;
    NAS_MML_SIM_MS_IDENTITY_STRU       *pstMsIdentity;

    pstMsIdentity           = NAS_MML_GetSimMsIdentity();

    /* SIM卡不在位 */
    if ( VOS_FALSE == NAS_MML_GetSimPresentStatus())
    {
        pstMsIdMsg->ucLength        = 0;
        return ;
    }

    pstMsIdMsg->ucLength            = 15;
    pstMsIdMsg->ucMobileIDValue[0]  = (pstMsIdentity->aucImsi[1] & 0xF0) >> 4;

    i   = 1;
    j   = 2;

    for ( ; (i < pstMsIdMsg->ucLength) && (j < 9); i++)
    {
        if ( (2 * i) >= LEN_MOBILE )
        {
            break;
        }

        pstMsIdMsg->ucMobileIDValue[(2 * i) - 1]
                    = pstMsIdentity->aucImsi[j] & 0x0F;

        pstMsIdMsg->ucMobileIDValue[2 * i]
                    = pstMsIdentity->aucImsi[j++] >> 4;
    }

    return;
}

/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasSimUpdateStatusInfo
 功能描述  : 填充OM查询消息的卡的更新状态,包括CS域和PS域的
 输入参数  : 无
 输出参数  : pstMsg:更新后的卡状态
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID NAS_MMC_FillNasSimUpdateStatusInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    NAS_MML_SIM_STATUS_STRU            *pstSimStatus;

    pstSimStatus                = NAS_MML_GetSimStatus();

    /* 填写PS域的卡状态 */
    switch (pstSimStatus->enPsUpdateStatus)
    {
        case NAS_MML_ROUTING_UPDATE_STATUS_UPDATED:
            pstMsg->ucGmmUpdateStatus   = UPDATE_STATUS_GU1;
            break;

        case NAS_MML_ROUTING_UPDATE_STATUS_NOT_UPDATED:
            pstMsg->ucGmmUpdateStatus   = UPDATE_STATUS_GU2;
            break;

        case NAS_MML_ROUTING_UPDATE_STATUS_PLMN_NOT_ALLOWED:
        case NAS_MML_ROUTING_UPDATE_STATUS_ROUTING_AREA_NOT_ALLOWED:
            pstMsg->ucGmmUpdateStatus   = UPDATE_STATUS_GU3;
            break;

        default:
            pstMsg->ucGmmUpdateStatus   = GMM_UPDATE_STATUS_BUTT;
            break;
    }


    /* 填写CS域的卡状态 */
    switch (pstSimStatus->enCsUpdateStatus)
    {
        case NAS_MML_LOCATION_UPDATE_STATUS_UPDATED:
            pstMsg->ucMmUpdateStatus    = UPDATE_STATUS_U1;
            break;

        case NAS_MML_LOCATION_UPDATE_STATUS_NOT_UPDATED:
            pstMsg->ucMmUpdateStatus    = UPDATE_STATUS_U2;
            break;

        case NAS_MML_LOCATION_UPDATE_STATUS_PLMN_NOT_ALLOWED:
        case NAS_MML_LOCATION_UPDATE_STATUS_LOCATION_AREA_NOT_ALLOWED:
            pstMsg->ucMmUpdateStatus    = UPDATE_STATUS_U3;
            break;

        default:
            pstMsg->ucMmUpdateStatus    = MM_UPDATE_STATUS_BUTT;
            break;
    }
}

/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasMmSimInfo
 功能描述  : 填充OM查询消息的SIM卡信息项
 输入参数  : 无
 输出参数  : pstMsg:当前查询内容
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/

VOS_VOID NAS_MMC_FillNasMmSimInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    VOS_UINT8                           i;
    NAS_MML_SIM_MS_IDENTITY_STRU       *pstMsIdentity;

    pstMsIdentity           = NAS_MML_GetSimMsIdentity();

    /* 填写PTMSI的内容 */
    for (i = 0; i < LEN_PTMSI; i++)
    {
        pstMsg->ucPtmsi[i]  = pstMsIdentity->aucPtmsi[i];
    }

    /* 填写TMSI的内容 */
    for (i = 0; i < LEN_TMSI; i++)
    {
        pstMsg->aucTmsi[i]  = pstMsIdentity->aucTmsi[i];
    }

    /* 填写IMSI的内容 */
    NAS_MMC_FillNasMmImsiInfo(&(pstMsg->IMSI));

    /* 填写卡更新状态 */
    NAS_MMC_FillNasSimUpdateStatusInfo(pstMsg);

    return;
}


/*****************************************************************************
 函 数 名  : NAS_MMC_ConvertNetworkModeToOm
 功能描述  : 将内部的网络模式信息转换为显示给OM的网络信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2011年11月12日
    作    者   : zhoujun 40661
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID  NAS_MMC_ConvertNetworkModeToDispalyFormat(
    NAS_MML_NET_MODE_ENUM_UINT8         enNetworkMode,
    NET_OPERATE_MODE_ENUM_UINT8        *penNetOperateMode

)
{
    if ( NAS_MML_NET_MODE_I == enNetworkMode )
    {
        *penNetOperateMode = NET_OPERATE_MODE_1;
    }
    else if ( NAS_MML_NET_MODE_II == enNetworkMode )
    {
        *penNetOperateMode = NET_OPERATE_MODE_2;
    }
    else if ( NAS_MML_NET_MODE_III == enNetworkMode )
    {
        *penNetOperateMode = NET_OPERATE_MODE_3;
    }
    else
    {
        *penNetOperateMode = NET_OPERATE_MODE_BUTT;
    }
}




NAS_OM_PLMN_HUO_TYPE_ENUM_UINT32 NAS_MMC_GetPlmnHUOType(NAS_MML_PLMN_ID_STRU *pstPlmn)
{
    NAS_MML_CAMP_PLMN_INFO_STRU                             *pstCampPlmnInfo;
    NAS_MML_PLMN_ID_STRU                                     stRPlmnId;

    if (VOS_FALSE == NAS_MML_IsPlmnIdValid(pstPlmn))
    {
        return NAS_MMC_HUO_INVALID_PLMN;
    }

    /* 获取RPLMN配置信息 */
    pstCampPlmnInfo         = NAS_MML_GetCurrCampPlmnInfo();

    if (VOS_TRUE == NAS_MMC_GetRPlmn_PlmnSelectionStrategy(pstCampPlmnInfo->enNetRatType, &stRPlmnId))
    {
        if (VOS_TRUE == NAS_MML_CompareBcchPlmnwithSimPlmn(pstPlmn, &stRPlmnId))
        {
            return NAS_MMC_HUO_RPLMN;
        }
    }

    /* 检查是否是HOME PLMN */
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithHplmn(pstPlmn))
    {
        return NAS_MMC_HUO_HPLMN;
    }

    /* 检查是否是 UPLMN */
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithUplmn(pstPlmn))
    {
        return NAS_MMC_HUO_UPLMN;
    }

    /* 检查是否是 OPLMN */
    if (VOS_TRUE == NAS_MML_ComparePlmnIdWithOplmn(pstPlmn))
    {
        return NAS_MMC_HUO_OPLMN;
    }

    /* 其他PLMN */
    return NAS_MMC_HUO_OTHER_PLMN;
}

VOS_VOID NAS_MMC_FillNasTinInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
#if (FEATURE_ON == FEATURE_LTE)
    NAS_LMM_INFO_STRU                   stLmmInfo;

    NAS_MMC_FSM_ID_ENUM_UINT32  enFsmId;
    VOS_UINT32                  ucMmcTopState;
    VOS_UINT8                   aucPtmsiAddr[20];

    pstMsg->enTinType     = NAS_MML_GetTinType();
    pstMsg->stMsId.ulMsId = 0;

    
    enFsmId         = (VOS_UINT8)NAS_MMC_GetCurrFsmId();
    ucMmcTopState   = (VOS_UINT8)NAS_MMC_GetFsmTopState();

   
    /*关机状态下返回无效值,不能调用NAS_Gmm_FillPtmsiIeForSnd这个接口*/
    if ( ( NAS_MMC_FSM_L1_MAIN == enFsmId )
      && ( NAS_MMC_L1_STA_NULL == ucMmcTopState ))  
    {
        pstMsg->stMsId.ulMsId = 0;
        PS_MEM_SET(&(pstMsg->stMsId.stGuti),0xFF,sizeof(pstMsg->stMsId.stGuti));
        PS_MEM_SET(&(pstMsg->stMsId.stPtmsi),0xFF,sizeof(pstMsg->stMsId.stPtmsi));
        PS_MEM_SET((pstMsg->stMsId.aucImsi),0x00,sizeof(pstMsg->stMsId.aucImsi));

        return;
    }

    if (VOS_FALSE == NAS_MML_GetSimPresentStatus())
    {
        return;
    }

    /* 根据TIN TYPE的值填写相应的TIN值 */
    if (NAS_MML_TIN_TYPE_PTMSI == pstMsg->enTinType)
    {
        NAS_Gmm_FillPtmsiIeForSnd(aucPtmsiAddr);        
        PS_MEM_CPY((pstMsg->stMsId).stPtmsi.aucPtmsi,&aucPtmsiAddr[2],NAS_MAX_SIZE_PTMSI);    
        pstMsg->stMsId.ulMsId |= NAS_MS_ID_PTMSI_IMSI_VALID;
    }
    else if (NAS_MML_TIN_TYPE_GUTI == pstMsg->enTinType)
    {
        if ( VOS_TRUE == NAS_GMM_GetLteInfo( NAS_LMM_GUTI, &stLmmInfo ) )
        {
            VOS_MemCpy(&(pstMsg->stMsId.stGuti), &(stLmmInfo.u.stGuti), sizeof(NAS_LMM_GUTI_STRU));
            pstMsg->stMsId.ulMsId |= NAS_MS_ID_GUTI_VALID;
        }
    }
    else if (NAS_MML_TIN_TYPE_RAT_RELATED_TMSI == pstMsg->enTinType)
    {
        NAS_Gmm_FillPtmsiIeForSnd(aucPtmsiAddr);        
        PS_MEM_CPY((pstMsg->stMsId).stPtmsi.aucPtmsi,&aucPtmsiAddr[2],NAS_MAX_SIZE_PTMSI);    
        pstMsg->stMsId.ulMsId |= NAS_MS_ID_PTMSI_IMSI_VALID;

        if ( VOS_TRUE == NAS_GMM_GetLteInfo( NAS_LMM_GUTI, &stLmmInfo ) )
        {
            VOS_MemCpy(&(pstMsg->stMsId.stGuti), &(stLmmInfo.u.stGuti), sizeof(NAS_LMM_GUTI_STRU));
            pstMsg->stMsId.ulMsId |= NAS_MS_ID_GUTI_VALID;
        }
    }
    else if (NAS_MML_TIN_TYPE_INVALID == pstMsg->enTinType)
    {
        NAS_Gmm_FillPtmsiIeForSnd(aucPtmsiAddr);        
        PS_MEM_CPY((pstMsg->stMsId).stPtmsi.aucPtmsi,&aucPtmsiAddr[2],NAS_MAX_SIZE_PTMSI);    
        pstMsg->stMsId.ulMsId |= NAS_MS_ID_PTMSI_IMSI_VALID;
    }
    else
    {
    }
    
#else
/*
    pstMsg->enTinType     = NAS_MML_TIN_TYPE_BUTT;
    pstMsg->stMsId.ulMsId = 0;
    NAS_Gmm_FillPtmsiIeForSnd( (pstMsg->stMsId).stPtmsi.aucPtmsi );
    pstMsg->stMsId.ulMsId |= NAS_MS_ID_PTMSI_IMSI_VALID;
    */
#endif
}



VOS_VOID NAS_MMC_FillNasMsNetworkInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    NAS_MML_CAMP_PLMN_INFO_STRU        *pstCampPlmnInfo;

    pstCampPlmnInfo         = NAS_MML_GetCurrCampPlmnInfo();

    /* 驻留网络信息 */


    pstMsg->ucRac           = pstCampPlmnInfo->ucRac;
    pstMsg->usLac           = ((pstCampPlmnInfo->stLai.aucLac[0] << 8) | (pstCampPlmnInfo->stLai.aucLac[1]));

    /* 搜网模式信息 */
    pstMsg->enPlmnSelMode   = NAS_MMC_GetPlmnSelectionMode();

    pstMsg->enPlmnType      = NAS_MMC_GetPlmnHUOType(&(pstCampPlmnInfo->stLai.stPlmnId));

    NAS_MMC_ConvertPlmnIdToOmDispalyFormat(&(pstCampPlmnInfo->stLai.stPlmnId),
                                           &(pstMsg->SelectedPlmn));


    NAS_MMC_ConvertNetworkModeToDispalyFormat(pstCampPlmnInfo->enNetworkMode,
                                             &(pstMsg->ucNetMode));


}

/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasMsMode
 功能描述  : 填充OM查询消息的手机模式信息项
 输入参数  : 无
 输出参数  : pstMsg:当前查询内容
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/

VOS_VOID NAS_MMC_FillNasMsMode(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    NAS_MML_MS_MODE_ENUM_UINT8          enMsMode;

    enMsMode    = NAS_MML_GetMsMode();

    switch ( enMsMode )
    {
        case NAS_MML_MS_MODE_NULL:
            pstMsg->enUeOperMode = UE_OPERATE_MODE_NULL;
            break;

        case NAS_MML_MS_MODE_PS_CS:
            pstMsg->enUeOperMode = UE_OPERATE_MODE_A;
            if (NAS_MML_NET_RAT_TYPE_GSM == NAS_MML_GetCurrNetRatType())
            {
                pstMsg->enUeOperMode = UE_OPERATE_MODE_B;
            }
            break;

        case NAS_MML_MS_MODE_PS_ONLY:
            pstMsg->enUeOperMode = UE_OPERATE_MODE_CG;
            break;

        case NAS_MML_MS_MODE_CS_ONLY:
            pstMsg->enUeOperMode = UE_OPERATE_MODE_CC;
            break;

        default:
            pstMsg->enUeOperMode = UE_OPERATE_MODE_BUTT;
            NAS_WARNING_LOG(WUEPS_PID_MMC,
            "NAS_MMC_FillNasMsMode:WARNING: enMsMod is Error");
            break;
    }
}

/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasImeiInfo
 功能描述  : 填充OM查询消息的IMEI信息项
 输入参数  : 无
 输出参数  : pstMsg:当前查询内容
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/

VOS_VOID NAS_MMC_FillNasImeiInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    VOS_UINT32                          i;
    VOS_UINT8                           ucCheckData;
    NAS_MML_MS_CAPACILITY_INFO_STRU    *pstMsCapacility;

    pstMsg->IMEI.ucLength   = LEN_MOBILE - 1;
    pstMsg->IMEISV.ucLength = LEN_MOBILE;
    pstMsCapacility         = NAS_MML_GetMsCapability();
    ucCheckData             = 0;

    for (i = 0; i < (LEN_MOBILE - 2); i += 2)
    {
        ucCheckData += (VOS_UINT8)((pstMsCapacility->aucImeisv[i]
                    + ((pstMsCapacility->aucImeisv[i + 1] * 2) / 10))
                    + ((pstMsCapacility->aucImeisv[i + 1] * 2) % 10));
    }
    ucCheckData = (10 - (ucCheckData % 10)) % 10;

    pstMsg->IMEI.ucMobileIDValue[14] = ucCheckData;
    pstMsg->IMEI.ucMobileIDValue[15] = 0;


    for (i = 0; i < (LEN_MOBILE - 2); i++)
    {
        pstMsg->IMEI.ucMobileIDValue[i]     = pstMsCapacility->aucImeisv[i];
        pstMsg->IMEISV.ucMobileIDValue[i]   = pstMsCapacility->aucImeisv[i];
    }

    pstMsg->IMEISV.ucMobileIDValue[14]      = pstMsCapacility->aucImeisv[14];
    pstMsg->IMEISV.ucMobileIDValue[15]      = pstMsCapacility->aucImeisv[15];

}


/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasMsCfgInfo
 功能描述  : 填充OM查询消息的手机配置信息项
 输入参数  : 无
 输出参数  : pstMsg:当前查询内容
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/

VOS_VOID NAS_MMC_FillNasMsCfgInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    /* 填写IMEI内容 */
    NAS_MMC_FillNasImeiInfo(pstMsg);


    /* 填写手机模式内容 */
    NAS_MMC_FillNasMsMode(pstMsg);
}

/*****************************************************************************
 函 数 名  : NAS_MMC_ConvertPlmnIdToOmDispalyFormat
 功能描述  : 将内部格式的PlmnID转换为OM显示用的格式
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID  NAS_MMC_ConvertPlmnIdToOmDispalyFormat(
    NAS_MML_PLMN_ID_STRU               *pstPlmnId,
    PLMN_ID_STRUCT                     *pstOmPlmnIdFormat
)
{
    pstOmPlmnIdFormat->aucMccDigit[0] = (VOS_UINT8)(pstPlmnId->ulMcc & 0x0F);
    pstOmPlmnIdFormat->aucMccDigit[1] = (VOS_UINT8)((pstPlmnId->ulMcc >> 8) & 0x0F);
    pstOmPlmnIdFormat->aucMccDigit[2] = (VOS_UINT8)((pstPlmnId->ulMcc >> 16) & 0x0F);

    pstOmPlmnIdFormat->aucMncDigit[0] = (VOS_UINT8)(pstPlmnId->ulMnc & 0x0F);
    pstOmPlmnIdFormat->aucMncDigit[1] = (VOS_UINT8)((pstPlmnId->ulMnc >> 8) & 0x0F);
    pstOmPlmnIdFormat->aucMncDigit[2] = (VOS_UINT8)((pstPlmnId->ulMnc >> 16) & 0x0F);

    pstOmPlmnIdFormat->ucMncCount = 3;

    if (0x0F == pstOmPlmnIdFormat->aucMncDigit[2])
    {
        pstOmPlmnIdFormat->ucMncCount = 2;
    }

}

/*****************************************************************************
 函 数 名  : NAS_MMC_FillNasServiceStatus
 功能描述  : 填写NAS内部服务状态
 输入参数  : 无
 输出参数  : pstMsg:待填写的消息内容
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2011年7月23日
   作    者   : zhoujun \40661
   修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID NAS_MMC_FillNasServiceStatus(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    NAS_MMC_SERVICE_INFO_CTX_STRU      *pstServiceStatus = VOS_NULL_PTR;

    pstServiceStatus = NAS_MMC_GetServiceInfo();

    if ( ( NAS_MMC_NORMAL_SERVICE == pstServiceStatus->enCsCurrService)
      || ( NAS_MMC_NORMAL_SERVICE == pstServiceStatus->enPsCurrService))
    {
        pstMsg->ucServiceStatus     = NAS_OM_SERVICE_STATE_NORMAL_SERVICE;
    }
    else if ( ( NAS_MMC_NO_SERVICE == pstServiceStatus->enCsCurrService)
           && ( NAS_MMC_NO_SERVICE == pstServiceStatus->enPsCurrService))
    {
        pstMsg->ucServiceStatus     = NAS_OM_SERVICE_STATE_NO_SERVICE;
    }
    else if ( ( NAS_MMC_LIMITED_SERVICE_REGION == pstServiceStatus->enCsCurrService)
           || ( NAS_MMC_LIMITED_SERVICE_REGION == pstServiceStatus->enPsCurrService))
    {
        pstMsg->ucServiceStatus     = NAS_OM_SERVICE_STATE_REGIONAL_LIMITED_SERVICE;
    }
    else
    {
        pstMsg->ucServiceStatus     = NAS_OM_SERVICE_STATE_LIMITED_SERVICE;
    }
}



VOS_VOID NAS_MMC_FillOmAvailPlmnList(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    VOS_UINT32                              i;
    VOS_UINT32                              j;
    VOS_UINT32                              ulPlmnCnt;
    AVAILABLE_PLMN_STRUCT                  *pstPlmnAddr = VOS_NULL_PTR;
    NAS_MML_NET_RAT_TYPE_ENUM_UINT8         enNetRat;
    NAS_MMC_NET_STATUS_ENUM_UINT8           enNetExistFlag;
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU  *pstPlmnSelectionList = VOS_NULL_PTR;

    ulPlmnCnt           = 0;
    enNetExistFlag      = NAS_MMC_NET_STATUS_BUTT;
    enNetRat            = NAS_MML_NET_RAT_TYPE_BUTT;

    /* 只有搜网状态机运行时才会存在可用网络列表 */
    if ( NAS_MMC_FSM_PLMN_SELECTION != NAS_MMC_GetCurrFsmId())
    {
        if (VOS_TRUE == NAS_MMC_IsNormalServiceStatus())
        {
            pstPlmnAddr = &(pstMsg->AvailablePlmn[ulPlmnCnt]);

            NAS_MMC_ConvertPlmnIdToOmDispalyFormat(NAS_MML_GetCurrCampPlmnId(),
                                                   &(pstPlmnAddr->Plmn));

            pstPlmnAddr->ucRat = NAS_MML_GetCurrNetRatType();

            ulPlmnCnt++;
        }

        pstMsg->ucAvailPlmnNum = (VOS_UINT8)ulPlmnCnt;

        return;
    }

    pstPlmnSelectionList = NAS_MMC_GetPlmnSelectionListInfo_PlmnSelection();

    for (i = 0; i < pstPlmnSelectionList->usSearchPlmnNum; i++)
    {
        for (j = 0; j < pstPlmnSelectionList->astPlmnSelectionList[i].ucRatNum; j++)
        {
            if ( ulPlmnCnt >= MAX_AVAILABEL_PLMN_NUM )
            {
                break;
            }

            /* 只需要导出存在的网络 */
            enNetExistFlag = pstPlmnSelectionList->astPlmnSelectionList[i].astPlmnRatInfo[j].enNetStatus;
            enNetRat       = pstPlmnSelectionList->astPlmnSelectionList[i].astPlmnRatInfo[j].enRatType;

            if  ( ( NAS_MMC_NET_STATUS_SEARCHED_EXIST     == enNetExistFlag)
               || ( NAS_MMC_NET_STATUS_SEARCHED_REGISTERED == enNetExistFlag ))
            {
                pstPlmnAddr = &(pstMsg->AvailablePlmn[ulPlmnCnt]);

                NAS_MMC_ConvertPlmnIdToOmDispalyFormat(&(pstPlmnSelectionList->astPlmnSelectionList[i].stPlmnId),
                                                       &(pstPlmnAddr->Plmn));

                pstPlmnAddr->ucRat = enNetRat;

                ulPlmnCnt++;

            }
        }
    }

    pstMsg->ucAvailPlmnNum = (VOS_UINT8)ulPlmnCnt;
}



VOS_VOID NAS_MMC_BuildNasOmInfo(
    NAS_OM_MM_CONFIRM_STRU             *pstMsg
)
{
    /* 填写SIM卡信息 */
    NAS_MMC_FillNasMmSimInfo(pstMsg);

    /* 填写MS 配置信息 */
    NAS_MMC_FillNasMsCfgInfo(pstMsg);

    /* 填写当前驻留网络信息 */
    NAS_MMC_FillNasMsNetworkInfo(pstMsg);

    /* 填写MM的状态和子状态 */
    NAS_MM_FillNasMmState(pstMsg);

    /* 填写GMM的状态和子状态 */
    NAS_GMM_FillNasGmmState(pstMsg);

    /* 填写TIN TYPE和MSID*/
    NAS_MMC_FillNasTinInfo(pstMsg);

    /* 填写MMC的状态和子状态 */
    /* 此处需要与OM讨论修改MMC内部接口 */
    pstMsg->ucMmcFsmId = (VOS_UINT8)NAS_MMC_GetCurrFsmId();
    pstMsg->ucMmcState = (VOS_UINT8)NAS_MMC_GetFsmTopState();

    /* 填写可用的网络 */
    NAS_MMC_FillOmAvailPlmnList(pstMsg);

    /* 填写当前服务状态 */
    NAS_MMC_FillNasServiceStatus(pstMsg);

}


VOS_VOID NAS_MMC_SndOmInquireCnfMsg(
    ID_NAS_OM_INQUIRE_STRU             *pstOmInquireMsg
)
{
    ID_NAS_OM_CNF_STRU                 *pstMmOmCnf  =   VOS_NULL_PTR;
    VOS_UINT8                          *pstSndData  =   VOS_NULL_PTR;


    pstSndData = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_MMC,
        (sizeof(ID_NAS_OM_CNF_STRU) + sizeof(NAS_OM_MM_CONFIRM_STRU)) - NAS_OM_DATA_PTR_LEN); /* 申请内存 */

    if (VOS_NULL_PTR == pstSndData)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmInquireCnfMsg:Memory Allocate fail!");
        return;
    }

    PS_MEM_SET((VOS_UINT8*)pstSndData, 0 ,(sizeof(ID_NAS_OM_CNF_STRU) - NAS_OM_DATA_PTR_LEN + sizeof(NAS_OM_MM_CONFIRM_STRU)));

    pstMmOmCnf = (ID_NAS_OM_CNF_STRU*)pstSndData;

    pstMmOmCnf->ucFuncType         = 4;
    pstMmOmCnf->ucReserve          = 0;
    pstMmOmCnf->usLength           = NAS_OM_DATA_OFFSET + sizeof(NAS_OM_MM_CONFIRM_STRU);
    pstMmOmCnf->ulSn               = 0;
    pstMmOmCnf->ulTimeStamp        = OM_GetSlice();
    pstMmOmCnf->usPrimId           = ID_NAS_OM_MM_CONFIRM;
    pstMmOmCnf->usToolsId          = pstOmInquireMsg->usToolsId;

    NAS_MMC_BuildNasOmInfo((NAS_OM_MM_CONFIRM_STRU*)(pstMmOmCnf->aucData));

    OM_SendData((OM_RSP_PACKET_STRU*)pstMmOmCnf, (sizeof(ID_NAS_OM_CNF_STRU) + sizeof(NAS_OM_MM_CONFIRM_STRU)) - NAS_OM_DATA_PTR_LEN);                                   /* 原语发送                                 */

    PS_MEM_FREE(WUEPS_PID_MMC, pstMmOmCnf);


}



VOS_VOID NAS_MMC_SndOmPlmnSelectionList(
    NAS_MMC_PLMN_SELECTION_LIST_INFO_STRU                  *pstPlmnSelectionList,
    NAS_MML_PLMN_RAT_PRIO_STRU                             *pstPrioRatList
)
{
    VOS_UINT32                                              i;
    VOS_UINT32                                              j;
    VOS_UINT32                                              k;
    VOS_UINT32                                              ulPlmnNum;
    NAS_MMC_LOG_PLMN_SELECTION_LIST_MSG_STRU               *pstMsg               = VOS_NULL_PTR;
    NAS_MMC_PLMN_SELECTION_PLMN_INFO_STRU                  *pstPlmnSelectionInfo = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_PLMN_SELECTION_LIST_MSG_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_PLMN_SELECTION_LIST_MSG_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogPlmnSelectionList_PlmnSelection:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_PLMN_SELECTION_LIST;

    ulPlmnNum = 0x0;

    for (i = 0; i < pstPlmnSelectionList->usSearchPlmnNum; i++)
    {
        if (i > NAS_MMC_MAX_PLMN_NUM_IN_SELECTION_LIST/NAS_MML_MAX_RAT_NUM)
        {
            break;
        }

        pstPlmnSelectionInfo = &(pstPlmnSelectionList->astPlmnSelectionList[i]);

        /* 按SYSCFG设置的接入技术优先级依次查找 */
        for (k = 0; k < pstPrioRatList->ucRatNum; k++)
        {
            /* 依次查找一个网络的所有支持的接入技术 */
            for (j = 0; j < pstPlmnSelectionInfo->ucRatNum; j++)
            {
                /* 如果优先的接入技术与当前查找的网络支持的接入技术相同 */
                if (pstPrioRatList->aucRatPrio[k] == pstPlmnSelectionInfo->astPlmnRatInfo[j].enRatType)
                {
                    pstMsg->astPlmnSelectionList[ulPlmnNum].ulMcc       = pstPlmnSelectionInfo->stPlmnId.ulMcc;
                    pstMsg->astPlmnSelectionList[ulPlmnNum].ulMnc       = pstPlmnSelectionInfo->stPlmnId.ulMnc;
                    pstMsg->astPlmnSelectionList[ulPlmnNum].enPlmnType  = pstPlmnSelectionInfo->enPlmnType;
                    pstMsg->astPlmnSelectionList[ulPlmnNum].enRatType   = pstPlmnSelectionInfo->astPlmnRatInfo[j].enRatType;
                    pstMsg->astPlmnSelectionList[ulPlmnNum].enNetStatus = pstPlmnSelectionInfo->astPlmnRatInfo[j].enNetStatus;
                    pstMsg->astPlmnSelectionList[ulPlmnNum].enQuality   = pstPlmnSelectionInfo->astPlmnRatInfo[j].enQuality;

                    ulPlmnNum ++;
                }
            }

        }
    }

    pstMsg->ulPlmnNum                   = ulPlmnNum;

    pstMsg->stMsgHeader.ulLength        = sizeof(pstMsg->stMsgHeader.ulMsgName)
                                          + sizeof(pstMsg->ulPlmnNum)
                                          + (ulPlmnNum * sizeof(NAS_MMC_LOG_PLMN_SELECTION_LIST_STRU));

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}
VOS_VOID NAS_MMC_LogRplmnRelatedInfo(VOS_VOID)
{
    NAS_MMC_LOG_RPLMN_RELATED_INFO_STRU                      *pstMsg  = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_RPLMN_RELATED_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_RPLMN_RELATED_INFO_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogRplmnRelatedInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    PS_MEM_SET(pstMsg, 0X00, sizeof(NAS_MMC_LOG_RPLMN_RELATED_INFO_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_RPLMN_RELATED_INFO;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_RPLMN_RELATED_INFO_STRU) - VOS_MSG_HEAD_LENGTH;

    /* 获取LTE的RPLMN */
    NAS_MML_GetLteRPlmn(&pstMsg->stGetLteRplmn);

    /* 获取GU的RPLMN */
    NAS_MML_GetGURPlmn(&pstMsg->stGetGuRplmn);   

    /* 打印RPLMN的配置信息 */
    PS_MEM_CPY(&pstMsg->stRplmnCfg, NAS_MML_GetRplmnCfg(), sizeof(NAS_MML_RPLMN_CFG_INFO_STRU));

    /* CS域最后一次注册成功的LAI信息或注册失败后需要删除LAI，则该值为无效值 */
    PS_MEM_CPY(&pstMsg->stLastSuccLai, NAS_MML_GetCsLastSuccLai(), sizeof(NAS_MML_LAI_STRU));

    /* PS域最后一次注册成功的RAI信息或注册失败后需要删除RAI，则该值为无效值 */
    PS_MEM_CPY(&pstMsg->stLastSuccRai, NAS_MML_GetPsLastSuccRai(), sizeof(NAS_MML_RAI_STRU));

    /* PS UPDATE STATUS */
    pstMsg->enPsUpdateStatus   = NAS_MML_GetPsUpdateStatus();

    /* CS UPDATE STATUS */
    pstMsg->enCsUpdateStatus   = NAS_MML_GetCsUpdateStatus();

    /* MS MODE */
    pstMsg->enMsMode            = NAS_MML_GetMsMode();

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}


VOS_VOID NAS_MMC_LogForbiddenPlmnRelatedInfo(VOS_VOID)
{
    NAS_MMC_LOG_FORBIDDEN_PLMN_RELATED_INFO_STRU           *pstMsg       = VOS_NULL_PTR;
    NAS_MML_CUSTOM_CFG_INFO_STRU                           *pstCustomCfg = VOS_NULL_PTR;

    pstCustomCfg = NAS_MML_GetCustomCfg();

    pstMsg = (NAS_MMC_LOG_FORBIDDEN_PLMN_RELATED_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_FORBIDDEN_PLMN_RELATED_INFO_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogForbiddenPlmnRelatedInfo:ERROR:Alloc Mem Fail.");
        
        return;
    }

    PS_MEM_SET(pstMsg, 0X00, sizeof(NAS_MMC_LOG_FORBIDDEN_PLMN_RELATED_INFO_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_FORBIDDEN_PLMN_RELATED_INFO;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_FORBIDDEN_PLMN_RELATED_INFO_STRU) - VOS_MSG_HEAD_LENGTH;

    /* 打印FORB PLMN的信息 */
    PS_MEM_CPY(&pstMsg->stSimForbidenInfo, NAS_MML_GetForbidPlmnInfo(), sizeof(NAS_MML_SIM_FORBIDPLMN_INFO_STRU));

    /* 打印ROMA CFG的信息 */
    PS_MEM_CPY(&pstMsg->stRoamCfg, NAS_MML_GetRoamCfg(), sizeof(NAS_MML_ROAM_CFG_INFO_STRU));

    /* 打印锁网定制需求,黑名单或白名单的信息 */
    PS_MEM_CPY(&pstMsg->stPlmnLockCfg, NAS_MML_GetPlmnLockCfg(), sizeof(NAS_MML_PLMN_LOCK_CFG_INFO_STRU));

    /* 打印LTE国际漫游的信息 */
    PS_MEM_CPY(&pstMsg->stLteRoamCfg, &(pstCustomCfg->stLteCustomCfgInfo.stLteRoamCfg), sizeof(NAS_MML_LTE_INTERNATION_ROAM_CFG_STRU));

    /* 打印禁止接入技术的信息 */
    PS_MEM_CPY(&pstMsg->stRatFirbiddenStatusCfg, &(pstCustomCfg->stRatFirbiddenStatusCfg), sizeof(NAS_MML_RAT_FORBIDDEN_STATUS_STRU));

    /* 打印DISABLE LTE的相关信息，在DISABLE LTE时对搜网会产生影响 */
    pstMsg->enLteCapabilityStatus = NAS_MML_GetLteCapabilityStatus();
    pstMsg->enDisableLteReason    = NAS_MML_GetDisableLteReason();
    pstMsg->ulDisableLteRoamFlg   = NAS_MML_GetDisableLteRoamFlg();

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}



VOS_VOID NAS_MMC_LogRplmnCfgInfo(VOS_VOID)
{
    NAS_MMC_LOG_RPLMN_CFG_INFO_STRU                      *pstMsg  = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_LOG_RPLMN_CFG_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                          (sizeof(NAS_MMC_LOG_RPLMN_CFG_INFO_STRU)));
    if (TAF_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_LogRplmnCfgInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    PS_MEM_SET(pstMsg, 0X00, sizeof(NAS_MMC_LOG_RPLMN_CFG_INFO_STRU) - VOS_MSG_HEAD_LENGTH);

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_RPLMN_CFG_INFO;
    pstMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_RPLMN_CFG_INFO_STRU) - VOS_MSG_HEAD_LENGTH;

    /* 打印RPLMN的配置信息 */
    PS_MEM_CPY(&pstMsg->stRplmnCfg, NAS_MML_GetRplmnCfg(), sizeof(NAS_MML_RPLMN_CFG_INFO_STRU));

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

    return;
}





VOS_VOID  NAS_MMC_SndOmMmcTimerStatus(
    NAS_MMC_TIMER_STATUS_ENUM_U8        enTimerStatus,
    NAS_MMC_TIMER_ID_ENUM_UINT16        enTimerId,
    VOS_UINT32                          ulTimerRemainLen
)
{
    NAS_MMC_TIMER_INFO_STRU            *pstMsg = VOS_NULL_PTR;

    pstMsg = (NAS_MMC_TIMER_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                             sizeof(NAS_MMC_TIMER_INFO_STRU));
    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmMmcTimerStart:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;

    pstMsg->stMsgHeader.ulLength        = NAS_MML_MSG_TIMER_ID_LENGTH + NAS_MML_MSG_NAME_ID_LENGTH;

    pstMsg->stMsgHeader.ulMsgName       = enTimerId;
    pstMsg->enTimerStatus               = enTimerStatus;
    pstMsg->enTimerId                   = enTimerId;
    pstMsg->ucReserve                   = 0;

    pstMsg->ulTimerRemainLen            = ulTimerRemainLen;

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

}


VOS_VOID NAS_MMC_SndOmEquPlmn(VOS_VOID)
{
    VOS_UINT8                          *pucEplmnContent = VOS_NULL_PTR;
    VOS_UINT32                          i;
    NAS_MML_EQUPLMN_INFO_STRU          *pstEplmnList = VOS_NULL_PTR;
    NAS_MML_SIM_FORMAT_PLMN_ID          stReportPlmn;                           /* 该Plmn与Sim格式相同 */
    VOS_UINT32                          ulSndEplmnNum;
    VOS_UINT32                          ulPlmnIdx;

    pstEplmnList  = NAS_MML_GetEquPlmnList();

    /* 上报RPlmn+Eplmn当前网络 */
    ulSndEplmnNum  = pstEplmnList->ucEquPlmnNum;

    if ( 0 == ulSndEplmnNum )
    {
        /* 为了发送当前网络 */
        ulSndEplmnNum = 1;
    }

    pucEplmnContent = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_MMC,
                                                NAS_MML_SIM_FORMAT_PLMN_LEN * ulSndEplmnNum);
    if( VOS_NULL_PTR == pucEplmnContent )
    {
        NAS_WARNING_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmEquPlmn():Memory Alloc Error for pucEplmnContent ");
        return;
    }

    PS_MEM_SET(pucEplmnContent, 0, ulSndEplmnNum * NAS_MML_SIM_FORMAT_PLMN_LEN);

    ulPlmnIdx = 0;

    if (pstEplmnList->ucEquPlmnNum > 0)
    {
        for (i = 0; i < pstEplmnList->ucEquPlmnNum; i++)
        {
            if (VOS_TRUE == NAS_MML_IsBcchPlmnIdInDestSimPlmnList(&(pstEplmnList->astEquPlmnAddr[i]),
                                               (VOS_UINT8)ulPlmnIdx, pstEplmnList->astEquPlmnAddr))
            {
                continue;
            }

            NAS_MMC_ConvertNasPlmnToSimFormat(&(pstEplmnList->astEquPlmnAddr[i]), &stReportPlmn);

            PS_MEM_CPY(pucEplmnContent + (ulPlmnIdx * NAS_MML_SIM_FORMAT_PLMN_LEN),
                      stReportPlmn.aucSimPlmn, NAS_MML_SIM_FORMAT_PLMN_LEN);

            ulPlmnIdx++;
        }
    }
    /* EPlmn不存在时，只添加当前网络 */
    else
    {
        NAS_MMC_ConvertNasPlmnToSimFormat(NAS_MML_GetCurrCampPlmnId(), &stReportPlmn);

        PS_MEM_CPY(pucEplmnContent + (ulPlmnIdx * NAS_MML_SIM_FORMAT_PLMN_LEN),
                  stReportPlmn.aucSimPlmn, NAS_MML_SIM_FORMAT_PLMN_LEN);

        ulPlmnIdx++;
    }


    NAS_EventReport(WUEPS_PID_MMC, NAS_OM_EVENT_EPLMN_LIST,
                    pucEplmnContent, (NAS_MML_SIM_FORMAT_PLMN_LEN * ulPlmnIdx));


    /* 释放内存 */
    PS_MEM_FREE(WUEPS_PID_MMC, pucEplmnContent);

    return;
}
VOS_VOID NAS_MMC_SndOmInternalMsgQueueDetailInfo(
    NAS_MML_INTERNAL_MSG_QUEUE_STRU    *pInternalMsgQueue
)
{
    VOS_UINT32                                              ulMsgPnt;
    VOS_UINT8                                               ucHeader;
    VOS_UINT8                                               ucTail;

    ucHeader = pInternalMsgQueue->ucHeader;
    ucTail   = pInternalMsgQueue->ucTail;

    /* 打印出消息队列中的内容 */
    for (ulMsgPnt = ucHeader; ulMsgPnt != ucTail; ulMsgPnt = (ulMsgPnt + 1)%NAS_MML_MAX_MSG_QUEUE_SIZE)
    {
        OM_TraceMsgHook(&pInternalMsgQueue->astNasMmMsgQueue[ulMsgPnt]);
    }

    return;
}
VOS_VOID  NAS_MMC_SndOmInternalMsgQueueInfo(
    VOS_UINT8                          ucFullFlg,
    VOS_UINT8                          ucMsgLenValidFlg
)
{
    NAS_MMC_LOG_INTER_MSG_INFO_STRU     *pstInterMsg = VOS_NULL_PTR;

    pstInterMsg = (NAS_MMC_LOG_INTER_MSG_INFO_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC,
                             sizeof(NAS_MMC_LOG_INTER_MSG_INFO_STRU));

    if ( VOS_NULL_PTR == pstInterMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_GMM, "NAS_MMC_SndOmInternalMsgQueueInfo:ERROR:Alloc Mem Fail.");
        return;
    }

    pstInterMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstInterMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstInterMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstInterMsg->stMsgHeader.ulLength        = sizeof(NAS_MMC_LOG_INTER_MSG_INFO_STRU) - VOS_MSG_HEAD_LENGTH;
    pstInterMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_INTER_MSG_INFO_IND;
    pstInterMsg->ucFullFlg                   = ucFullFlg;
    pstInterMsg->ucMsgLenValidFlg            = ucMsgLenValidFlg;

    OM_TraceMsgHook(pstInterMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstInterMsg);

    return;
}


VOS_VOID  NAS_MMC_SndOmGetCacheFile(
    VOS_UINT32                          ulFileId,
    VOS_UINT32                          ulFileLen,
    VOS_UINT8                          *pucFileContent
)
{
    NAS_MMC_GET_CACHE_FILE_STRU        *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;

    ulMsgLen = sizeof(NAS_MMC_GET_CACHE_FILE_STRU) + ulFileLen;

    pstMsg = (NAS_MMC_GET_CACHE_FILE_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, ulMsgLen);

    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmGetCacheFile:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulLength        = ulMsgLen - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = ulFileId;

    pstMsg->ulFileLen                   = ulFileLen;

    PS_MEM_CPY(pstMsg->aucFileContent, pucFileContent, ulFileLen);

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

}

VOS_VOID  NAS_MMC_SndDrxTimerInfo( VOS_UINT8 ucDrxTimerStatus )
{
    NAS_MMC_DRX_TIMER_STAUTS_STRU        *pstMsg = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;

    ulMsgLen = sizeof(NAS_MMC_DRX_TIMER_STAUTS_STRU)  ;

    pstMsg = (NAS_MMC_DRX_TIMER_STAUTS_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, ulMsgLen);

    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmGetCacheFile:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulLength        = ulMsgLen - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_DRX_TIMER_STATUS_IND;
    PS_MEM_SET(pstMsg->aucReserve,0x00,sizeof(pstMsg->aucReserve));
    pstMsg->ucDrxTimerStatus            = ucDrxTimerStatus;

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

}


VOS_VOID  NAS_MMC_SndOmPlatformRatCap( VOS_VOID )
{
    NAS_MMC_PLATFORM_RAT_CAP_STRU          *pstMsg = VOS_NULL_PTR;
    NAS_MML_PLATFORM_RAT_CAP_STRU          *pstPlatformRatCap =VOS_NULL_PTR;
    VOS_UINT32                              ulMsgLen;

    pstPlatformRatCap   = NAS_MML_GetPlatformRatCap();
    ulMsgLen            = sizeof(NAS_MMC_PLATFORM_RAT_CAP_STRU);

    pstMsg = (NAS_MMC_PLATFORM_RAT_CAP_STRU*)PS_MEM_ALLOC(WUEPS_PID_MMC, ulMsgLen);

    if (VOS_NULL_PTR == pstMsg)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndOmPlatformRatCap:ERROR:Alloc Mem Fail.");
        return;
    }

    pstMsg->stMsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->stMsgHeader.ulSenderPid     = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulReceiverPid   = WUEPS_PID_MMC;
    pstMsg->stMsgHeader.ulLength        = ulMsgLen - VOS_MSG_HEAD_LENGTH;
    pstMsg->stMsgHeader.ulMsgName       = MMCOM_LOG_MMC_PLATFORM_RAT_CAP;

    PS_MEM_CPY(&(pstMsg->stPlatformRatCap), pstPlatformRatCap, sizeof(NAS_MML_PLATFORM_RAT_CAP_STRU));

    OM_TraceMsgHook(pstMsg);

    PS_MEM_FREE(WUEPS_PID_MMC, pstMsg);

}


#if (FEATURE_ON == FEATURE_PTM)

VOS_VOID NAS_MMC_SndAcpuOmErrLogRptCnf(
    VOS_CHAR                           *pbuffer,
    VOS_UINT32                          ulBufUseLen
 )
{
    OM_ERR_LOG_REPORT_CNF_STRU         *pstQryCnf = VOS_NULL_PTR;
    VOS_UINT32                          ulMsgLen;

    /* 上报的消息总长度 */
    ulMsgLen  = sizeof(OM_ERR_LOG_REPORT_CNF_STRU) - VOS_MSG_HEAD_LENGTH - 4 + ulBufUseLen;

    /* 消息空间申请 */
    pstQryCnf = (OM_ERR_LOG_REPORT_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_MMC, ulMsgLen);
    if (VOS_NULL_PTR == pstQryCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndAcpuOmErrLogRptCnf: Alloc msg fail!");
        return;
    }

    pstQryCnf->ulReceiverCpuId  = VOS_LOCAL_CPUID;
    pstQryCnf->ulSenderCpuId    = VOS_LOCAL_CPUID;
    pstQryCnf->ulReceiverPid    = ACPU_PID_OM;
    pstQryCnf->ulSenderPid      = WUEPS_PID_MMC;
    pstQryCnf->ulMsgName        = ID_OM_ERR_LOG_REPORT_CNF;
    pstQryCnf->ulMsgType        = OM_ERR_LOG_MSG_ERR_REPORT;
    pstQryCnf->ulMsgSN          = 0;
    pstQryCnf->ulRptlen         = ulBufUseLen;

    if (VOS_NULL_PTR != pbuffer)
    {
        PS_MEM_CPY(pstQryCnf->aucContent, pbuffer, ulBufUseLen);
    }

    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_MMC, pstQryCnf))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndAcpuOmErrLogRptCnf: SEND MSG FAIL");
    }

    return;

}


VOS_VOID NAS_MMC_SndAcpuOmFtmRptInd(
    VOS_UINT8                          *pucData,
    VOS_UINT32                          ulLen
)
{
    VOS_UINT32                          ulMsgLen;
    OM_FTM_REPROT_IND_STRU             *pstFtmRptInd = VOS_NULL_PTR;

    /* 申请消息结构内存 */
    ulMsgLen     = sizeof(OM_FTM_REPROT_IND_STRU) - VOS_MSG_HEAD_LENGTH - 4 + ulLen;
    pstFtmRptInd = (OM_FTM_REPROT_IND_STRU*)PS_ALLOC_MSG(WUEPS_PID_MMC, ulMsgLen);
    if (VOS_NULL_PTR == pstFtmRptInd)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndAcpuOmFtmRptInd: Alloc msg fail!");
        return;
    }

    /* 构造消息结构体 */
    pstFtmRptInd->ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstFtmRptInd->ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstFtmRptInd->ulReceiverPid     = ACPU_PID_OM;
    pstFtmRptInd->ulSenderPid       = WUEPS_PID_MMC;
    pstFtmRptInd->ulMsgName         = ID_OM_FTM_REPROT_IND;
    pstFtmRptInd->ulMsgType         = OM_ERR_LOG_MSG_FTM_REPORT;
    pstFtmRptInd->ulMsgSN           = 0;
    pstFtmRptInd->ulRptlen          = ulLen;

    PS_MEM_CPY(pstFtmRptInd->aucContent, pucData, ulLen);

    /* 发送消息到ACPU OM模块 */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_MMC, pstFtmRptInd))
    {
        NAS_ERROR_LOG(WUEPS_PID_MMC, "NAS_MMC_SndAcpuOmFtmRptInd: SEND MSG FIAL");
    }

    return;

}
VOS_VOID NAS_MMC_SndAcpuOmCurTmsi(VOS_VOID)
{
    VOS_UINT8                          *pucTmsi = VOS_NULL_PTR;
    NAS_FTM_TMSI_RPT_EVENT_STRU         stFtmTmsiRpt;

    /* 读取当前的TMSI */
    pucTmsi  = NAS_MML_GetUeIdTmsi();

    /* 填充工程模式上报的头 */
    NAS_COMM_BULID_FTM_HEADER_INFO(&stFtmTmsiRpt.stHeader,
                                   NAS_MML_MAX_TMSI_LEN,
                                   VOS_GetModemIDFromPid(WUEPS_PID_MMC),
                                   NAS_FTM_PROJECT_TMSI_RPT)

    PS_MEM_CPY(stFtmTmsiRpt.aucTmsi,   pucTmsi,  NAS_MML_MAX_TMSI_LEN);

    /* 通过消息ID_OM_FTM_REPROT_IND发送给ACPU OM */
    NAS_MMC_SndAcpuOmFtmRptInd((VOS_UINT8*)&stFtmTmsiRpt,  sizeof(stFtmTmsiRpt));

    return;
}
VOS_VOID NAS_MMC_SndAcpuOmCurPtmsi(VOS_VOID)
{
    VOS_UINT8                          *pucPtmsi = VOS_NULL_PTR;
    NAS_FTM_PTMSI_RPT_EVENT_STRU        stFtmPtmsiRpt;

    /* 读取当前的TMSI/PTMSI */
    pucPtmsi = NAS_MML_GetUeIdPtmsi();

    /* 填充工程模式上报的头 */
    NAS_COMM_BULID_FTM_HEADER_INFO(&stFtmPtmsiRpt.stHeader,
                                   NAS_MML_MAX_PTMSI_LEN,
                                   VOS_GetModemIDFromPid(WUEPS_PID_MMC),
                                   NAS_FTM_PROJECT_PTMSI_RPT)

    PS_MEM_CPY(stFtmPtmsiRpt.aucPtmsi, pucPtmsi, NAS_MML_MAX_PTMSI_LEN);

    /* 通过消息ID_OM_FTM_REPROT_IND发送给ACPU OM */
    NAS_MMC_SndAcpuOmFtmRptInd((VOS_UINT8*)&stFtmPtmsiRpt, sizeof(stFtmPtmsiRpt));
}

#endif


#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

