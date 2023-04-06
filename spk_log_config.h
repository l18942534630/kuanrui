/*
 * Copyright 2009-2016 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    spk_log_config.h
 *
 * 日志配置文件解析处理头文件(非线程安全)
 *
 * @version $Id$
 * @since   2005.10.31
 */


#ifndef _SPK_LOG_CONFIG_H
#define _SPK_LOG_CONFIG_H


#include    <sutil/types.h>
#include    <sutil/logger/spk_log_type.h>
#include    <sutil/logger/spk_log_level.h>
#include    <sutil/logger/spk_log_mode.h>


#ifdef __cplusplus
extern "C" {
#endif


/*
 * 常量定义
 */
/** 最大的日志配置数量 */
#define     SLOGCFG_MAX_CONFIG_COUNT            (32)
/** 最大的日志记录器数量 */
#define     SLOGCFG_MAX_APPENDER_NUMBER         (16)
/** 日志配置区段(日志分类)名称的最大长度 */
#define     SLOGCFG_MAX_CATEGORY_NAME_LENGTH    (64)
/* -------------------------           */


/*
 * 日志默认配置定义
 */
#define     SLOGCFG_DEFAULT_MIN_LOG_LEVEL       SLOG_LEVEL_TRACE
#define     SLOGCFG_DEFAULT_MAX_FILE_LENGTH     (400 * 1024 * 1024)
#define     SLOGCFG_DEFAULT_MAX_BACKUP_COUNT    (3)
#define     SLOGCFG_DEFAULT_ASYNC_QUEUE_SIZE    (1000)
#define     SLOGCFG_DEFAULT_ASYNC_SHM_ID        (0)
/* -------------------------           */


/*
 * 配置文件常量定义
 */
/** 默认的日志根配置区段 */
#define     SLOGCFG_KEY_DEFAULT_ROOT_SECTION    "log"

/** 默认的日志根配置 */
#define     SLOGCFG_KEY_DEFAULT_ROOT_CATEGORY   "log.root_category"

/** 多值域的域分隔符 */
#define     SLOGCFG_MULTI_VALUE_DELIMITER       ","
/* -------------------------           */


/*
 * 结构体定义
 */

/**
 * 日志配置信息结构
 */
typedef struct _SLogCfgItem {
    /** 日志配置区段(日志分类)名称 */
    char                logCategory[SLOGCFG_MAX_CATEGORY_NAME_LENGTH];

    /**
     * 日志模式
     * - 取值说明:
     *   - FILE              : 文件日志 - 等同于 FILE_ROLLING (轮换, 不区分具体日期)
     *   - FILE_ROLLING      : 文件日志 - 轮换, 不区分具体日期
     *   - FILE_DAILY        : 文件日志 - 每天一个日志文件
     *   - FILE_DAILY_ROLLING: 文件日志 - 每天N个日志文件 (N >= 1)
     *   - CONSOLE           : 控制台日志 - 等同于 CONSOLE_STDOUT (输出到标准输出)
     *   - CONSOLE_STDOUT    : 控制台日志 - 输出到标准输出 (stdout)
     *   - CONSOLE_STDERR    : 控制台日志 - 输出到标准错误 (stderr)
     *   - ASYNC_FILE        : 异步文件日志
     */
    char                logMode[SLOG_MAX_MODE_NAME];
    /**
     * 日志登记的起始级别
     * - 日志级别列表 (级别从低到高):
     *   - 跟踪信息 (TRACE)
     *   - 调试信息 (DEBUG)
     *   - 提示信息 (INFO)
     *   - 警告信息 (WARN)
     *   - 错误信息 (ERROR)
     *   - 业务提示 (BZINF, BZ_INFO)
     *   - 业务警告 (BZWRN, BZ_WARN)
     *   - 业务错误 (BZERR, BZ_ERROR)
     *   - 致命错误 (FATAL)
     *   - 屏蔽所有日志 (NONE)
     */
    char                minLogLevel[SLOG_MAX_LEVEL_NAME];
    /** 日志文件路径 */
    char                logFile[SPK_MAX_PATH_LEN];

    /**
     * 日志文件最大长度
     * - 取值小于0, 表示无最大长度限制
     * - 取值等于0, 将使用默认值 (400M)
     * - 如果取值小于2048, 将以兆为单位计算, 否则以字节为单位计算, 最大文件长度为2GB
     */
    int32               maxFileLength;
    /**
     * 日志文件最大备份数
     * - 轮换(ROLLING)模式下的最大保留的历史日志文件数量
     * - 取值小于0, 表示不保留备份文件
     * - 取值等于0, 将使用默认值 (3)
     */
    int32               maxBackupCount;

    /** 异步日志的消息队列大小 (仅适用于异步日志) */
    int32               asyncQueueSize;
    /** 异步日志的共享内存ID (仅适用于异步日志, 小于等于0表示使用默认值) */
    int32               asyncQueueShmId;
} SLogCfgItemT;


/* 结构体初始化值定义 */
#define NULLOBJ_LOG_CFG_ITEM                    \
        {0}, {0}, {0}, {0}, \
        0, 0, \
        0, 0
/* -------------------------           */


/**
 * 日志配置区段(日志分类)信息
 */
typedef struct _SLogCategoryName {
    /** 日志配置区段(日志分类)名称 */
    char                logCategory[SLOGCFG_MAX_CATEGORY_NAME_LENGTH];
} SLogCategoryNameT;
/* -------------------------           */


/*
 * 函数声明
 */

/*
 * 尝试加载所有的日志配置区段
 */
int32   SLogCfg_LoadAllConfig(
                const char *pConfigFile,
                SLogCfgItemT *pOutLogConfigList,
                int32 maxLogConfigCount);

/*
 * 解析日志配置信息, 读取相关的日志分类列表，并加载所有的日志配置区段
 */
int32   SLogCfg_LoadAllConfigAndCategoryList(
                const char *pConfigFile,
                const char *pRootSection,
                const char *pRootCategoryField,
                SLogCfgItemT *pOutLogConfigList,
                int32 *pLogConfigCount,
                SLogCategoryNameT *pOutLogCategoryList,
                int32 *pLogCategoryCount,
                const SLogLevelT **ppOutAllowableMinLogLevel);
/* -------------------------           */


#ifdef __cplusplus
}
#endif

#endif  /* _SPK_LOG_CONFIG_H */
