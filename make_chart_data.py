# random,1310720,google_dense_hash_map,45621248,0.344362020493
# random,2621440,glib_hash_table,109867008,1.01163601875
# random,2621440,stl_unordered_map,130715648,1.73484396935
# random,2621440,boost_unordered_map,108380160,1.11585187912
# random,2621440,google_sparse_hash_map,37015552,1.76031804085
# random,2621440,google_dense_hash_map,79175680,0.504401922226
# random,5242880,glib_hash_table,210530304,1.86031603813
# random,5242880,stl_unordered_map,250298368,3.81597208977
# random,5242880,boost_unordered_map,192184320,2.63760495186
# random,5242880,google_sparse_hash_map,62066688,3.93570995331
# random,5242880,google_dense_hash_map,146284544,1.22620105743
# random,10485760,glib_hash_table,411856896,4.16937494278
# random,10485760,stl_unordered_map,490430464,7.91806197166
# random,10485760,boost_unordered_map,359251968,7.52085900307
# random,10485760,google_sparse_hash_map,111902720,8.11318516731
# random,10485760,google_dense_hash_map,280502272,2.32930994034
# random,20971520,glib_hash_table,814510080,8.32456207275
# random,20971520,stl_unordered_map,971583488,16.1606841087
# random,20971520,boost_unordered_map,692441088,24.5845990181
# random,20971520,google_sparse_hash_map,211435520,16.2772600651
# random,20971520,google_dense_hash_map,548937728,4.85360789299
# random,41943040,glib_hash_table,1619816448,90.6313672066

import sys, json

lines = [ line.strip() for line in sys.stdin if line.strip() ]

by_benchtype = {}

proper_names = {}

for line in lines:
    if (line.startswith("identify")):
        benchtype, program, proper_name, color = line.split(',')
        proper_names[ program ] = proper_name
        continue

    benchtype, nkeys, program, nbytes, runtime = line.split(',')
    nkeys = int(nkeys)
    nbytes = int(nbytes)
    runtime = float(runtime)

    by_benchtype.setdefault("%s-runtime" % benchtype, {}).setdefault(program, []).append([nkeys, runtime])
    if benchtype.startswith('sequential'):
        by_benchtype.setdefault("%s-memory"  % benchtype, {}).setdefault(program, []).append([nkeys, nbytes])

# sort programs in the desired order inside the 'Targets' file to 
# make the legend not overlap the chart data too much
program_slugs = [ line.strip() for line in open('Targets', 'r') if line.strip() ]

chart_data = {}

maxes = { 'time': 0.0, 'keys': 0, 'mem': 0 }
for i, (benchtype, programs) in enumerate(by_benchtype.items()):
    chart_data[benchtype] = []
    for j, program in enumerate(program_slugs):
        if not proper_names.has_key(program):
        	proper_names[program] = program

        data = programs[program]
        chart_data[benchtype].append({
            'label': proper_names[program],
            'data': [],
        })

        axisX = 'keys'
        axisY = 'time'
        if benchtype.endswith('memory'):
            axisY = 'mem'

        for k, (nkeys, value) in enumerate(data):
            chart_data[benchtype][-1]['data'].append([nkeys, value])
            maxes[ axisX ] = max( maxes[ axisX ], nkeys ) #if there's a more
            maxes[ axisY ] = max( maxes[ axisY ], value ) #pythonesqe way,tell me

# pick correct granularity for millions, thousands, ..., etc numbers of keys
tick_divide_by = 1000000
tick_divide_attr = 'M'
sizes = { 1000000: 'M', 100000: '00K', 10000: '0K', 1000: 'K', 100: '00', 1: '' }
for i, (divider, attr) in enumerate(sizes.items()):
    if (maxes[ 'keys' ] / divider > 0):
        tick_divide_by = divider
        tick_divide_attr = attr
        break 
# output it in a rather ugly way
print """
    xaxis_settings = {
        tickSize: %d,
        tickFormatter: function(num, obj) { return parseInt(num/%d) + '%s'; }
    };

    yaxis_runtime_settings = {
        tickSize: %0.1f,
        tickFormatter: function(num, obj) { return num + ' sec.'; }
    };

    yaxis_memory_settings = {
        tickSize: %d *1024*1024,
        tickFormatter: function(num, obj) { return parseInt(num/1024/1024) + 'MiB'; }
    };
    """ % (
            (maxes[ 'keys' ] / 10),	# 1 tick = 1/10 of maximum keys
            tick_divide_by,  tick_divide_attr,
            maxes[ 'time' ] , # top time is simply maximum time
            (maxes[ 'mem' ] /1024/1024)	# memory is always in MiBs
        )

print 'chart_data = ' + json.dumps(chart_data)
