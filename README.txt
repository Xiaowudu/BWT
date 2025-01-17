1. bwtfile.h/bwtfile.c
    This file is used to define macros, handling the index of c-table and file path.
2. bwtencode.c
    1) bwt encode
        Using quick-sort to sort the chars and get the indices of F column, then save BWT form F column.
    2) auxiliary file
        The auxiliray file is saving the indentifier of delimiter(from 0) in bwt encoded order.
3. bwtsearch.c
    1) -m (backward search)
	From the last char of search pattern, get the first and last position of rows matching the pattern[i], then calculate the previous char's position by cTablea nd Occ: 
	first = cTable[c] + Occ(c, first - 1) + 1;
	last = cTable[c] + Occ(c, last);
	if(last < first), then there is no matching, else (last-first+1) is the number of matching substrings.
    2) -n
        The first part is same with -m using backward search.
        The second part is to remove dupilicate records: From position first to position last of the results (with the dupilicate), it seeks the previous char using cTable and Occ. If a delimiter occurs, it continues to next position. If it meets a position between first and last, it removes this duplicate record. If the position has already been removed, then continues to the next postion.
    3) -a
	Similar with -n for the backward search and removing duplicates. 
        When meets a delimiter, it gets the number of occurance of this delimiter in BWT and gets the indentifier from the auxiliary file.
    4) -i
        As the delimiter is sorted in order in the F column, the algorithm finds the previous char using cTable and Occ from the k-th(form i-th to j-th) delimiter until it meets a delimiter
	

