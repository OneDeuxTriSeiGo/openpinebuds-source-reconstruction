# Multi-modal V200Z-R

* [简介]()

* [目录]()

* [相关仓]()

# 简介

## 开发板概述

Multi-modal V200Z-R 开发板是由 [湖南欧智通科技有限公司](http://www.fn-link.com/)出品的，基于[恒玄科技 BES2600WM](http://www.bestechnic.com/)芯片专为OpenHarmony系统打造的高性能、多功能、高性价比AIOT SOC 开发板，单模组集成四核ARM处理器（最高主频1GHz）,板载高度集成双频WiFi + 双模蓝牙，支持802.11a/b/g/n/协议，支持BT/BLE 5.2 协议，内建多种容量的RAM（最大42M）和 Flash（最大32M），支持MIPI DSI 及CSI，适用于各种AIOT多模态VUI+GUI交互硬件场景，具有高性能低功耗特性，功耗处于业界一流水平，丰富的接口可以外接LED,传感器等外设，芯片已经广泛应用于多种音箱。


## 开发板功能
Multi-modal V200Z-R，面向开发者，用于OpenHarmony开发学习，同时提供丰富案例和教程，实现全场景应用设计。

1. mipi-dsi：板载的屏幕驱动，可以流畅地在低功耗地屏幕场景下和用户交互，帧率最高可达60帧每秒，使得产品变得更加有趣便携，案例开发变得更加灵活和方便。

2. wifi：板载的wifi射频电路，配合OpenHarmony开放的commucation关键能力，可以完美实现OpenHarmony系统的万物互联的能力，加上蓝牙可以实现远程操控的家电的有趣场景。

3. 蓝牙：板载的蓝牙射频电路，配合OpenHarmony开放的commucation关键能力，可以完美实现OpenHarmony系统室内互联的能力，配合wifi可以实现更多有趣的远程场景。

4. 独立核唤醒：板载的A核，配合OpenHarmony系统 以及第三方 asr 能力，可以完美实现OpenHarmony系统有趣的Aiot场景。

5. 独立核voip：板载的A核，配合OpenHarmony 蓝牙可以完美实现voip功能，解放用户双手。

6. 用户按键：开放式功能按键，按键功能全权由开发者定义，增加了用户可操作性。

## 开发板详情
开发板详细功能如下图所示：

![](http://www.bestechnic.com/Uploads/Picture/2017-12-14/5a32465e496b7.jpg)

## 应用案例场景<a name="section1464106163819"></a>

### 开发板应用场景

Multi-modal V200Z-R可用于智能加湿器、智能台灯、智能安防、智能烟感、智能音箱、智能手表手环、智能面板、智能平板，智能电视等案例，如以下案例。

1. 智能加湿器：可实时监测室内当前的温湿度，可实现远程实时开启和关闭加湿器。

2. 智能台灯: 可实时监测当前的室内光照强度，低于预定的阈值时，自动开启台灯，并可实现对台灯的单独控制。

3. 智能安防：可实时监测范围内人体的移动，并上报云端显示，实现智能安防的监测和警报。

4. 智能烟感：可实时监测当前房间中的烟雾浓度，并上报云端显示，当烟雾浓度超过设定的阈值时，报警器立即触发警报。

5. 智能音箱：可实时陪老人小孩，说话，唱歌，讲故事，甚至可以语音控制家里的任何互联设备。

6. 智能手表手环：可在低功耗场景下，通过丰富的ui制作动画和游戏等，可以将手机的一部分消息处理能力赋能给Aiot芯片，特别适合开会人员。

7. 智能面板：可在室内室外携带，便携地控制家电，基于低功耗以及Aiot能力，特别适合家电升级增加智能控制面板。

8. 智能平板：可替代传统平板的唤醒功能，以及蓝牙wifi的能力，在功耗上更加优秀。

9. 智能电视：可替代传统电视的唤醒功能，以及蓝牙wifi的能力，做出不一样的效果和方案。


# 目录

```
device/bestechnic/bes2600w/
├── burn_tools          # 烧录工具包目录
│   ├── auto_build_tool # 生成hash和signature文件目录
│   ├── release         # 未生成hash和signature文件目录
│   ├── tools           # 处理烧录bin和烧录包的工具目录
│   ├── write_flash_gui # 基础烧录包的目录
├── components          # 组件服务层代码目录
│   ├── config          # hcs配置文件目录
│   ├── drivers         # 驱动适配目录
│   ├── fs              # 文件系统适配目录
│   ├── ui              # 升级ui界面代码目录
│   ├── net             # net的适配代码目录
│   └── utils           # 组件系统适配目录
├── hals                # hals适配目录
│   ├── commucation     # 连接类接口适配目录
│   ├── iot_hardware    # iot硬件适配目录
│   └── utils           # 系统相关接口适配目录
└── sdk_liteos          # 升级子系统通用代码目录
    ├── bsp             # bes2600w编译bsp目录
    └── tools           # 编译bsp使用的脚本和宏配置目录
```

# 说明 

适配鸿蒙编译集成bsp的目录

# 相关仓

* [device/bestechnic](https://gitee.com/openharmony-sig/device_bestechnic)

