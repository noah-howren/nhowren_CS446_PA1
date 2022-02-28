from re import A
import sys
def main():
    map_file_name = '/proc/cpuinfo'
    try:
        map_file = open('/proc/cpuinfo', 'r')
    except IOError as e:
            print('[ERROR] Cannot oepn file {}'.format(map_file_name))
            sys.exit(1)
        
    for line in map_file:
        f = open("nhowren_systemDetails.txt", 'a')
        f.write(line)


    map_file_name = '/proc/version'
    try:
        map_file = open('/proc/version', 'r')
    except IOError as e:
            print('[ERROR] Cannot oepn file {}'.format(map_file_name))
            sys.exit(1)
        
    for line in map_file:
        f = open("nhowren_systemDetails.txt", 'a')
        f.write(line)
    
    
    map_file_name = '/proc/uptime'
    try:
        map_file = open('/proc/uptime', 'r')
    except IOError as e:
            print('[ERROR] Cannot oepn file {}'.format(map_file_name))
            sys.exit(1)
        
    for line in map_file:
        f = open("nhowren_systemDetails.txt", 'a')
        f.write(line)
    

    map_file_name = '/proc/diskstats'
    try:
        map_file = open( '/proc/diskstats', 'r')
    except IOError as e:
            print('[ERROR] Cannot oepn file {}'.format(map_file_name))
            sys.exit(1)
        
    for line in map_file:
        f = open("nhowren_systemDetails.txt", 'a')
        f.write(line)


    map_file_name = '/proc/stat'
    try:
        map_file = open( '/proc/stat', 'r')
    except IOError as e:
            print('[ERROR] Cannot oepn file {}'.format(map_file_name))
            sys.exit(1)
        
    for line in map_file:
        f = open("nhowren_systemDetails.txt", 'a')
        f.write(line)

if __name__ == '__main__':
    main()