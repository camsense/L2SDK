# lcm2ros

## 编译方法

1. 安装转发工具，该工具可以接收扫地机转发的消息
lcm-bot-tunnel // in lcm-launch/tunnel, build with buildx86.sh
#### lcm2ros.sh
rosrun lcm2ros lcm2ros.sh [ip]
```

`lcm2ros.sh`负责开启`ros_to_lcm`、`lcm_to_ros`和`bot-lcm-tunnel`，最后参数为扫地机ip的最后一位。如扫地机ip为192.168.1.19，则输入ip为19即可。若收不到扫地机的消息，可能原因有以下几点：

- PC上开启了公司的VPN，或者不在一个局域网内，通过`ping`可以测试是否属于该情况
- 扫地机上没有开启或正确开启`bot-lcm-tunnel`，具体参考`lcm-launch/nav.sh`
- PC可以ping通，但是不在一个局域网内(常见于虚拟机)，需要手动修改`lcmTunnel.sh`内的`DEST_IP`


