#!/bin/sh
RED='\033[0;31m\033[1m'
NC='\033[0m'

signal_handler() {
    echo "${RED}Receive SIGINT, Kill lcm Tunnel...${NC}"
    exit
}

trap signal_handler INT
trap signal_handler TERM

TOPIC_SEND="slam_cmd|clean_mode|initialpose|goal"
TOPIC_RECV=""

if [ -e /usr/local/bin/bot-lcm-tunnel ]; then
    echo "Run LCM Tunnel ..."
    pidof bot-lcm-tunnel | xargs -r kill -9

    if [ $1 = '667' ]; then
        DEST_IP=127.0.0.1
    else
        BASE_IP=$(ifconfig | grep -oP '(192|10)(\.\d+){2}\.' | head -1)
        DEST_IP=$BASE_IP$1 # cleanRobot's ip
    fi

    LCM_URL=$(printf "udpm://239.255.76.67:7%03d" $1)

    echo "${RED} = Server at $DEST_IP${NC}"
    echo "${RED} = LCM_URL: $LCM_URL${NC}"

    while :
    do
        bot-lcm-tunnel $DEST_IP -p 12345 -m 2500 -s $TOPIC_SEND -l $LCM_URL
        # bot-lcm-tunnel $DEST_IP -p 12345 -m 2500 -s $TOPIC_SEND -R "backup_map|slam_map" -l $LCM_URL
        if [ $? -eq 137 ]; then
            exit
        fi
        sleep 1.5
    done

else
    echo "${RED} = Please Install bot-lcm-tunnel${NC}"
fi
