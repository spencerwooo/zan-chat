# Zan Chat 

[![license](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
![love](https://img.shields.io/badge/Made%20with-love-ff69b4.svg)
![Linux](https://img.shields.io/badge/Ubuntu%20&%20Arch%20Linux-Optimized-orange.svg)

A peer-to-peer communication system. BIT 小学期软件开发实训。

> Zan Chat | 本项目经由赞大佬亲启认证。

![img](https://i.loli.net/2018/09/05/5b8f8ffd4942d.png)

## 项目使用 Usage

项目在 Linux 环境下进行编译，实现了局域网下的点对点通讯的 **五大基础功能、四大用户界面设计功能和六大进阶功能。**

#### 我们实现了以下基础功能。

- 实现了局域网下收发消息的基础功能。
- 实现了用户登录制度，会员邀请制。
- 实现了局域网下新增 IP 登录嗅探。
- 实现了用户名称、用户头像、用户个性签名的同步和更改。
- 实现了用户在线状态的显示，同局域网下在线用户的 IP 显示。
- 实现了用户组别的设置。

#### 我们新增了以下用户设计。

- 新增了自行设计的表情 50 只，实现了收发表情的基础功能。
- 新增了 10 套用户头像，实现了用户头像的实时更新。
- 新增了 4 套界面皮肤，命名为「素鼠」「利休白茶」「中红新橘」和「露草」，简约美观，大方明丽。
- 默认界面与 Gnome - Arc Theme 深度合作，实现了沉浸式的界面显示。

#### 我们新增了以下更多功能。

- 聊天界面实现了字体的自行调教，全局字体渲染。
- 聊天界面实现了表情包全局显示和挑选，50 只原创表情直接挑选。
- 聊天界面实现了窗口截屏，利用 Linux 私有截屏 API 对屏幕截屏进行保存。
- 聊天界面实现了日历功能，直接调用 Linux 系统日历。
- 聊天界面实现了计算器功能，直接调用 Linux 系统计算器。
- 聊天界面实现了背景自定义，通过 Linux 系统调色盘对聊天背纯色直接定义。


## 项目依赖 Dependencies

项目在 Linux 下编译，依赖于 C 的基础语言，Gtk+-2.0 的原生基础图形库，进行界面设计。利用 Linux socket 技术实现局域网下的点对点通讯功能。

具体依赖全部路径如下：

```
/usr/include/gtk-2.0
/usr/lib/gtk-2.0/include
/usr/include/pango-1.0
/usr/include/glib-2.0
/usr/lib/glib-2.0/include
/usr/include/fribidi
/usr/include/cairo
/usr/include/pixman-1
/usr/include/freetype2
/usr/include/libpng16
/usr/include/harfbuzz
/usr/include/uuid
/usr/include/gdk-pixbuf-2.0
/usr/include/atk-1.0
```

> 项目依赖库经由 `pkg-config --cflags gtk+-2.0` 命令生成。

## 编译运行 Build

在项目根目录下执行 `bash run.sh` 进行项目的整体编译。

© Zan Chat Team 2018