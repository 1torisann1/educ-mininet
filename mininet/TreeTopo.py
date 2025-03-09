from mininet.net import Mininet
from mininet.topo import Topo
from mininet.cli import CLI
from mininet.log import setLogLevel
from mininet.node import NullController
from mininet.link import Intf
from mininet.link import TCLink

from NodeClass import HostV4, SwitchV4, CloudV4, MyNetwork

HOST_NUM = 2
DELAY = '10ms' # リンク遅延
BW = 10 #  帯域幅

class SmartCityTopo(Topo):
    def build(self):
        # switches
        topSwitch = self.addSwitch('s1', cls=SwitchV4)
        upSwitch = self.addSwitch('s2', cls=SwitchV4)
        middleSwitch1 = self.addSwitch('s3', cls=SwitchV4)
        middleSwitch2 = self.addSwitch('s4', cls=SwitchV4)
        downSwitch1 = self.addSwitch('s5', cls=SwitchV4)
        downSwitch2 = self.addSwitch('s6', cls=SwitchV4)
        downSwitch3 = self.addSwitch('s7', cls=SwitchV4)
        downSwitch4 = self.addSwitch('s8', cls=SwitchV4)

        # switch to switch links
        self.addLink(upSwitch, topSwitch, intfName1='s2-eth2', intfName2='s1-eth0', bw=BW, delay=DELAY)
        self.addLink(upSwitch, middleSwitch1, intfName1='s2-eth0', intfName2='s3-eth2', bw=BW, delay=DELAY)
        self.addLink(upSwitch, middleSwitch2, intfName1='s2-eth1', intfName2='s4-eth2', bw=BW, delay=DELAY)
        self.addLink(middleSwitch1, downSwitch1, intfName1='s3-eth0', intfName2='s5-eth2', bw=BW, delay=DELAY)
        self.addLink(middleSwitch1, downSwitch2, intfName1='s3-eth1', intfName2='s6-eth2', bw=BW, delay=DELAY)
        self.addLink(middleSwitch2, downSwitch3, intfName1='s4-eth0', intfName2='s7-eth2', bw=BW, delay=DELAY)
        self.addLink(middleSwitch2, downSwitch4, intfName1='s4-eth1', intfName2='s8-eth2', bw=BW, delay=DELAY)
        
        # hosts
        idx = 0
        for downSwitch in [downSwitch1, downSwitch2, downSwitch3, downSwitch4]:
            for i in range(HOST_NUM):
                hostName = f"h{idx * HOST_NUM + i}"
                macAddress = '00:00:00:00:' + format(idx, '02x') + ':' + format(i + 1, '02x')
                userHost = self.addHost(hostName, cls=HostV4, mac=macAddress, ip=f"10.0.{idx}.{i + 1}")
                # インターフェース名を動的に生成
                intfName1 = f"{hostName}-eth0"  # ホスト側のインターフェース名
                intfName2 = f"{downSwitch}-eth{i}"  # スイッチ側のインターフェース名
                self.addLink(userHost, downSwitch, intfName1=intfName1, intfName2=intfName2, bw=BW, delay=DELAY)
            idx += 1
        cloudHost = self.addHost('h20', cls=CloudV4, mac='00:00:00:02:00:20', ip='10.0.2.20')
        self.addLink(cloudHost, topSwitch, intfName1='h20-eth0', intfName2='s1-eth1', bw=BW, delay=DELAY)
        
if __name__ == '__main__':
    setLogLevel( 'info' )
    net = MyNetwork(topo=SmartCityTopo(), controller=NullController, link=TCLink)

    net.start()

    # CLIに移行
    CLI(net)

    net.stop()
