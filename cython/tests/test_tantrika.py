import _tantrika
print( "[INFO ] Using tantrika from %s" % _tantrika.__file__ )

for i, x in enumerate([x for x in dir(_tantrika.Network) if '__' not in x]):
    print(f'\t{i}: {x}')
    i += 1

n = _tantrika.Network('net1')
n.SynapseGroup(10, 1e-9, 1e-3, 0.0)
n.start(1)
