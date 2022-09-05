# 有源NFC标签组件<a name="ZH-CN_TOPIC_0000001124412109"></a>

-   [简介](#section11660541593)
-   [系统架构](#section342962219551)
-   [目录](#section161941989596)
-   [约束](#section119744591305)
-   [说明](#section1312121216216)
-   [使用说明](#section129654513264)
-   [相关仓](#section1371113476307)

## 简介<a name="section11660541593"></a>

近距离无线通信技术\(Near Field Communication，NFC\) ，是一种非接触式识别和互联技术，可以在移动设备、消费类电子产品、PC和智能设备间进行近距离无线通信。

针对具备有源NFC标签的产品，本组件提供有源NFC标签的读和写能力。

## 系统架构<a name="section342962219551"></a>

**图 1**  有源NFC标签架构图<a name="fig444313176464"></a>  


![](figures/zh-cn_image_0000001124327253.png)

<a name="table1716174620235"></a>
<table><thead align="left"></thead>
<tbody></tbody>
</table>

## 目录<a name="section161941989596"></a>

```
/foundation/communication/connected_nfc_tag
├── interfaces                        # 接口
│   └── inner_api                     # 系统内部件间接口
├── frameworks                        # 框架层接口
│   └── js                            # JS API的实现
│       └── napi                      # 通过napi封装的JS接口代码实现
├── services                          # NFC有源标签服务进程的实现
├── test                              # 测试代码
├── BUILD.gn                          # 编译入口
└── bundle.json                       # 部件描述文件
```

## 约束<a name="section119744591305"></a>

设备必须具备有源NFC标签芯片，才能使用有源NFC标签的读和写服务。

## 说明<a name="section1312121216216"></a>

### 使用说明<a name="section129654513264"></a>

-  有源NFC标签的读写

参考开发指南: docs/zh-cn/application-dev/reference/apis/js-apis-connectedTag.md。

## 相关仓<a name="section1371113476307"></a>

hmf/communication/connected_nfc_tag

