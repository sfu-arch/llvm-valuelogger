

def main():

    with open('Init.mem', 'r') as f, open('Init-update.mem', 'w') as g:

        address_map = 0x8FFFF0

        for line in f:
            if '@' in line:
                hex_addr = '0x' + line[1:].rstrip("\n\r")
                ex_int = int(hex_addr, 16)
                new_addr = '@' + str(hex(ex_int - address_map))[2:] + '\n'
                g.writelines(new_addr)
            else:
                g.writelines(line)

    with open('Args.mem', 'r') as f, open('Args-update.mem', 'w') as g:

        for line in f:
            if not line.strip(): continue
            else:
                arg  = line.rstrip("\n\r").split(':')
                arg_hex  = '0x' + arg[1]
                arg_int = int(arg_hex, 16)
                new_arg = str(hex(((arg_int >> 2) - address_map) << 2)) + '\n'
                print(arg[0] + ': ' + new_arg[2:])
                g.writelines(arg[0] + ': ' + new_arg[2:])




if __name__ == "__main__":
    main()

