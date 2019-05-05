import click
import curses
import sys, traceback, termios, tty, os, time
import csv_parser # Is part of grapher
import numpy as np
import matplotlib.pyplot as plt

button_delay = 0.25

@click.command()
@click.argument('csvfile', type=click.Path(exists=True))
def cli(csvfile):
    click.echo(click.format_filename(csvfile))
    data = csv_parser.parse_data(csvfile)
    fields = choose_fields(list(data.keys()))
    plot_data(fields, data)

def plot_data(fields, data):
    # Plot the selected fields
    # subplot(xyz) where x=numrows,y=numcols,z=plot_number
    # plot_number goes up to x*y
    rows = len(fields)
    cols = 1
    #fig = plt.figure(num=1)
    fig, axes = plt.subplots(rows, cols, sharex=True)
    plt.gca().grid(True, linewidth=0.7, linestyle=':')
    for idx,field in enumerate(fields):
        x = range(0,len(data[field]), 1) # no of data points 
        y = data[field]
        # Check data types
        if isinstance(y[0],str) and y[0].isdecimal():
            y = list(map(int, y)) # Turn all y into ints
            y_min = min(y)
            y_max = max(y) + 1
            y_step = (y_max - y_min) / 10
        elif isinstance(y[0],str) and not y[0].isdecimal():
            print("Data is string but not a decimal string,  of type:", type(y[0]))
            exit(0)
        else:
            print("Data is of type:", type(y[0]))
            print("y[0]:",y[0])
            exit(0)
        # Set up each subplot 
        axes[idx].plot(x,y)
        axes[idx].set_ylabel(field)
        #axes[idx].yticks(np.arange(y_min, y_max, y_step))
        """
        plt.subplot(rows, cols, idx+1)
        plt.plot(x, y)
        plt.ylabel(field)
        plt.yticks(np.arange(y_min, y_max, y_step))
        """
    mng = plt.get_current_fig_manager()
    mng.full_screen_toggle()
    ax_list = fig.axes
    plt.show()

def choose_fields(fields):
    chosen_fields = []
    field = ''
    possible_fields = []
    stdscr = curses.initscr()
    curses.cbreak()
    curses.noecho()
    stdscr.keypad(True)
    def print_fields(fields, field):
        stdscr.clear()
        stdscr.addstr("Enter empty field to exit")
        # If a user's string is a substring of an element in <fields>,
        # print it separated by newlines
        possible_fields = [s for s in fields if field in s]
        try:
            if len(possible_fields) == 1:
                stdscr.addstr("\n")
                stdscr.addstr("\n".join(possible_fields), curses.A_BLINK)
            else:
                stdscr.addstr("\n")
                stdscr.addstr("\n".join(possible_fields))
            stdscr.addstr("\n>:%s" % field)
        except:
            # Sometimes this throws an error
            pass

    stdscr.addstr("Variable(s) to graph:")
    for idx, f in enumerate(fields):
        if idx == 0:
            stdscr.addstr(f)
        else:
            stdscr.addstr(", " + f)
    stdscr.addstr("\n")

    try:
        while True:
            char = stdscr.getkey()

            if ord(char) in [curses.KEY_DC, 127]:
                # This is a backspace
                if len(field) > 0:
                    field = field[:-1]
                print_fields(fields, field)
            elif ord(char) in [curses.KEY_ENTER, 10] and sum(field in s for s in fields) == 1:
                # There is only one instance of field in fields
                # Grab the field that the user's input has matched to 
                chosen_fields += list(filter(lambda x: field in x, fields))
                # Reset the field, ready to choose again
                field = ''
                print_fields(fields, field)
            elif ord(char) in [curses.KEY_ENTER, 10] and field == '':
                # String is empty and user pressed enter so exit
                stdscr.clear()
                break
            elif ord(char) in [curses.KEY_ENTER, 10] and field not in fields:
                # User pressed enter and no field is ready to be chosen,
                # so clear the field
                field = ''
                print_fields(fields, field)
            else:
                # Build the string
                field += str(char)
                print_fields(fields, field)
    except:
        curses.nocbreak()
        stdscr.keypad(False)
        curses.echo()
        curses.endwin()
        print("Exception caught, exitting...")
        print("-"*60)
        traceback.print_exc(file=sys.stdout)
        print("-"*60)
    curses.nocbreak()
    stdscr.keypad(False)
    stdscr.clear()
    curses.echo()
    curses.endwin()
    return chosen_fields 
