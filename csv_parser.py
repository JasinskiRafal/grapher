import sys, csv

def get_reader(input_file):
    # Auto-grabs the dialect and opens the file
    opened_file = open(input_file, 'r')
    this_dialect = csv.Sniffer().sniff(opened_file.readline(), ',')
    opened_file.seek(0)
    reader = csv.reader(opened_file, delimiter=',', dialect=this_dialect)
    return reader, opened_file 

def parse_csv_data(input_file):
    reader, opened_file = get_reader(input_file)
    # Grabs header data
    fields = reader.__next__()
    data = {}
    buf = []
    for row_idx,row in enumerate(reader):
        # Goes down to next row
        for col_idx,field in enumerate(fields):
            # Goes across columns (fields)
            if row_idx == 0:
                # First run-thru is a special case
                data[field] = [row[col_idx]]
            else:
                buf = data[field]
                buf.append(row[col_idx])
                data[field] = buf
    opened_file.close()
    # Return a dict with keys corresponding to values
    return data   

def parse_log_csv_data(input_file):
    reader, opened_file = get_reader(input_file)
    # Grab header data
    raw_data = []
    date_time = []
    var_names = []
    for row in reader:
        raw_data.append(row)

    for idx,col in enumerate(raw_data[0]):
        # Go thru first row col by col
        if idx == 0:
            date_time.append(col)
        elif len(col) > 1:
            # Strip whitespace
            var_names.append(col.strip())
    opened_file.close()

def parse_data(input_file):
    if input_file.endswith(".log.csv"):
        data = parse_log_csv_data(input_file)
    else:
        data = parse_csv_data(input_file)
    return data

if __name__=="__main__":
    input_file = ''
    if len(argv) - 1 != 1:
        print("usage is python3 grapher.py <csv>")
        sys.exit(2)
    else:
        input_file = argv[1]
    # Gets a dictionary of the csv values
    data = parse_data(input_file)
