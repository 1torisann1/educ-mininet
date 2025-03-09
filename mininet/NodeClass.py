from mininet.node import Switch, Host
from mininet.nodelib import LinuxBridge
from mininet.net import Mininet
from mininet.link import TCLink

import time
import os

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