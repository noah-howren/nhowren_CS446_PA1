fin = open("/proc/cpuinfo", "r")
procCPUINFO= fin.readlines()

fin = open("/proc/version", "r")
procDataVersion = fin.readlines()

fin = open('/proc/uptime', 'r')
procUptime = fin.readlines()

fin = open('/proc/diskstats', 'r')
procDiskStats = fin.readlines()

fin = open('/proc/stat', 'r')
procStat = fin.readlines()

out = open("nhowren_systemDetails.txt", "w")
out.writelines(procCPUINFO)
out.writelines(procDataVersion) 
out.writelines(procUptime)
out.writelines(procDiskStats)
out.writelines(procStat)

fin.close()
out.close()