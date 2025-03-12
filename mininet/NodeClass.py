from mininet.node import Switch, Host
from mininet.nodelib import LinuxBridge
from mininet.net import Mininet
from mininet.link import TCLink

import time
import os
import config


LOG_DIR = "./log"
TCP_CLOUD_EXEC_FILE = "../host/TcpServer"
UDP_CLOUD_EXEC_FILE = "../host/UdpServer"

# switchをnamespaceに入れるときも入れないときも使用可能
class MyNetwork(Mininet):
    def configureControlNetwork(self):
        "Configure control network."
        pass

    def ping_test(self, num=5):
        time.sleep(3)
        start = time.time()
        for i in range(num):
            result = self.ping()
            if result != 0:
                print("Test Fail")
                break
        elapsed_time = time.time() - start
        print ("elapsed_time:{0}".format(elapsed_time) + "[sec]")
        
class HostV4(Host):
    def __init__(self, *args, **kwargs):
        super( HostV4, self ).__init__(*args, **kwargs)
        cfgs = [ 'all.disable_ipv6=1', 'default.disable_ipv6=1',
                 'default.autoconf=0', 'lo.autoconf=0' ]
        for cfg in cfgs:
            self.cmd( 'sysctl -w net.ipv6.conf.' + cfg )

class CloudV4(Host):
    def __init__(self, *args, **kwargs):
        super(CloudV4, self).__init__(*args, **kwargs)
        cfgs = ['all.disable_ipv6=1', 'default.disable_ipv6=1',
                'default.autoconf=0', 'lo.autoconf=0']
        for cfg in cfgs:
            self.cmd('sysctl -w net.ipv6.conf.' + cfg)

    def config(self, **kwargs):
        super(CloudV4, self).config(**kwargs)
        # LOG_DIR が存在しない場合は作成
        if not os.path.exists(LOG_DIR):
            os.makedirs(LOG_DIR, exist_ok=True)
            
        # CLOUD_EXEC_FILEをバックグラウンドで実行し、ログを出力
        log_file = os.path.join(LOG_DIR, f"{self.name}.log")
        error_log_file = os.path.join(LOG_DIR, f"{self.name}-error.log") 
        # if config.USE_TCP:
        #     self.cmd(f"{TCP_CLOUD_EXEC_FILE} >> {log_file} 2>> {error_log_file} &")
        # else:
        #     self.cmd(f"{UDP_CLOUD_EXEC_FILE} >> {log_file} 2>> {error_log_file} &")

class SwitchV4(LinuxBridge):
    def __init__(self, *args, **kwargs):
        super( SwitchV4, self ).__init__(*args, **kwargs)
        cfgs = [ 'all.disable_ipv6=1', 'default.disable_ipv6=1',
                 'default.autoconf=0', 'lo.autoconf=0' ]
        for cfg in cfgs:
            self.cmd( 'sysctl -w net.ipv6.conf.' + cfg )
        self.controlIntf = None

        # switchをnamespaceに入れない時に、これをしないとlinuxbridgeは動かなくなる(mininet起動時の警告は消えないが)
        self.cmd('sysctl -w net.bridge.bridge-nf-call-arptables=0')
        self.cmd('sysctl -w net.bridge.bridge-nf-call-iptables=0')
        self.cmd('sysctl -w net.bridge.bridge-nf-call-ip6tables=0')