import time
import argparse
import zenoh

from dataclasses import dataclass

from cyclonedds.idl import IdlStruct
from cyclonedds.idl.types import int8, uint8, int16, uint16, int32, uint32, int64, uint64, sequence, array
from cyclonedds.idl.annotations import key, keylist

# struct KeyedSeq
# {
#   unsigned long seq;
#   @key
#   unsigned long keyval;
#   sequence<octet> baggage;
# };
@dataclass
@keylist(["keyval"])
class KeyedSeq(IdlStruct, typename="KeyedSeq"):
    seq: uint32
    keyval: uint32
    baggage: sequence[uint8]


def chatter_callback(sample: zenoh.Sample):
    # Deserialize the message
    msg = KeyedSeq.deserialize(sample.payload)
    print('time: {t}, size: {size}'.format(t=time.time(), size=len(msg.baggage)))

def main():
    parser = argparse.ArgumentParser(
        prog='listener',
        description='Cyclone DDS listener example')
    parser.add_argument('--config', '-c', dest='config',
        metavar='FILE',
        type=str,
        help='A configuration file.')
    args = parser.parse_args()

    # Create Zenoh Config from file if provoded, or a default one otherwise
    conf = zenoh.Config.from_file(args.config) if args.config is not None else zenoh.Config()
    # Open Zenoh Session
    session = zenoh.open(conf)

    # Declare a subscriber
    sub = session.declare_subscriber('DDSPerfRDataKS', chatter_callback)

    try:
        while True:
            time.sleep(1)
    finally:
        sub.undeclare()
        session.close()


if __name__ == '__main__':
    main()
