import _tantrika
print( "[INFO ] Using tantrika from %s" % _tantrika.__file__ )

for i, x in enumerate([x for x in dir(_tantrika.Network) if '__' not in x]):
    print(f'\t{i}: {x}')
    i += 1

n = _tantrika.Network('/net')
n.addSynapse('a1')

syns = n.getSynapses()
print(syns)
