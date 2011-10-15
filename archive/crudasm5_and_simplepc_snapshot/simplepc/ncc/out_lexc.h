// out_lexc.h
// Copyright (C) 2009 Willow Schlanger

goto L_1066c8;
L_1066c8:
	switch(charAt(curOffset))
	{
		case '{':
			++curOffset;
			goto L_107f48;
		case '}':
			++curOffset;
			goto L_107fd8;
		case '[':
			++curOffset;
			goto L_108048;
		case ']':
			++curOffset;
			goto L_1080b8;
		case '#':
			++curOffset;
			goto L_108128;
		case '?':
			++curOffset;
			goto L_108198;
		case '.':
			++curOffset;
			goto L_108208;
		case '(':
			++curOffset;
			goto L_108358;
		case ')':
			++curOffset;
			goto L_1083c8;
		case ';':
			++curOffset;
			goto L_107568;
		case ':':
			++curOffset;
			goto L_108438;
		case '+':
			++curOffset;
			goto L_1085c8;
		case '-':
			++curOffset;
			goto L_108798;
		case '*':
			++curOffset;
			goto L_1089f8;
		case '/':
			++curOffset;
			goto L_108a68;
		case '%':
			++curOffset;
			goto L_108ba8;
		case '^':
			++curOffset;
			goto L_108e18;
		case '&':
			++curOffset;
			goto L_108ea8;
		case '|':
			++curOffset;
			goto L_109038;
		case '~':
			++curOffset;
			goto L_1091a8;
		case '!':
			++curOffset;
			goto L_109218;
		case '=':
			++curOffset;
			goto L_1092b8;
		case '<':
			++curOffset;
			goto L_109418;
		case '>':
			++curOffset;
			goto L_109718;
		case ',':
			++curOffset;
			goto L_1098a8;
		case '\\':
			++curOffset;
			goto L_109918;
		case ATEOF:
			return TOKEN_EOF;
		default:
			return TOKEN_UNKNOWN;
	}
L_107f48:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '{';
	}
L_107fd8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '}';
	}
L_108048:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '[';
	}
L_1080b8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ']';
	}
L_108128:
	switch(charAt(curOffset))
	{
		case '#':
			++curOffset;
			goto L_109988;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '#';
	}
L_108198:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '?';
	}
L_108208:
	switch(charAt(curOffset))
	{
		case '*':
			++curOffset;
			goto L_109d08;
		case '.':
			++curOffset;
			goto L_10a6a8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '.';
	}
L_108358:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '(';
	}
L_1083c8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ')';
	}
L_107568:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ';';
	}
L_108438:
	switch(charAt(curOffset))
	{
		case '>':
			++curOffset;
			goto L_1095b8;
		case ':':
			++curOffset;
			goto L_109c98;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ':';
	}
L_1085c8:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_109d78;
		case '+':
			++curOffset;
			goto L_10a4e8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '+';
	}
L_108798:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_109de8;
		case '-':
			++curOffset;
			goto L_10a558;
		case '>':
			++curOffset;
			goto L_10a5c8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '-';
	}
L_1089f8:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_109e58;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '*';
	}
L_108a68:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_109ec8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '/';
	}
L_108ba8:
	switch(charAt(curOffset))
	{
		case '>':
			++curOffset;
			goto L_109a68;
		case ':':
			++curOffset;
			goto L_109b48;
		case '=':
			++curOffset;
			goto L_109f38;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '%';
	}
L_108e18:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_109fa8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '^';
	}
L_108ea8:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a018;
		case '&':
			++curOffset;
			goto L_10a408;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '&';
	}
L_109038:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a088;
		case '|':
			++curOffset;
			goto L_10a478;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '|';
	}
L_1091a8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '~';
	}
L_109218:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a328;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '!';
	}
L_1092b8:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a2b8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '=';
	}
L_109418:
	switch(charAt(curOffset))
	{
		case '%':
			++curOffset;
			goto L_1099f8;
		case ':':
			++curOffset;
			goto L_109ad8;
		case '<':
			++curOffset;
			goto L_10a0f8;
		case '=':
			++curOffset;
			goto L_10a398;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '<';
	}
L_109718:
	switch(charAt(curOffset))
	{
		case '>':
			++curOffset;
			goto L_10a168;
		case '=':
			++curOffset;
			goto L_109818;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '>';
	}
L_1098a8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ',';
	}
L_109918:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '\\';
	}
L_109988:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_NUMNUM;
	}
L_1099f8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '{';
	}
L_109a68:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '}';
	}
L_109ad8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '[';
	}
L_1095b8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return ']';
	}
L_109b48:
	switch(charAt(curOffset))
	{
		case '%':
			++curOffset;
			goto L_109bb8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return '#';
	}
L_109bb8:
	switch(charAt(curOffset))
	{
		case ':':
			++curOffset;
			goto L_109c28;
		case ATEOF:
			unexpectedEndOfFile(curOffset);
		default:
			unexpectedChar(curOffset);
	}
L_109c28:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_NUMNUM;
	}
L_109c98:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x1);
			return TOKEN_SCOPE;
	}
L_109d08:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x1);
			return TOKEN_DOTSTAR;
	}
L_109d78:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_PLUSEQ;
	}
L_109de8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_MINUSEQ;
	}
L_109e58:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_STAREQ;
	}
L_109ec8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_DIVEQ;
	}
L_109f38:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_MODEQ;
	}
L_109fa8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_XOREQ;
	}
L_10a018:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_ANDEQ;
	}
L_10a088:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_OREQ;
	}
L_10a0f8:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a1d8;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_SHL;
	}
L_10a168:
	switch(charAt(curOffset))
	{
		case '=':
			++curOffset;
			goto L_10a248;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_SHR;
	}
L_10a1d8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_SHLEQ;
	}
L_10a248:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_SHREQ;
	}
L_10a2b8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_EQEQ;
	}
L_10a328:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_NOTEQ;
	}
L_10a398:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_LE;
	}
L_109818:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_GE;
	}
L_10a408:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_ANDAND;
	}
L_10a478:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_OROR;
	}
L_10a4e8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_PLUSPLUS;
	}
L_10a558:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_MINUSMINUS;
	}
L_10a5c8:
	switch(charAt(curOffset))
	{
		case '*':
			++curOffset;
			goto L_10a638;
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_POINT;
	}
L_10a638:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x1);
			return TOKEN_POINTSTAR;
	}
L_10a6a8:
	switch(charAt(curOffset))
	{
		case '.':
			++curOffset;
			goto L_1082b8;
		case ATEOF:
			unexpectedEndOfFile(curOffset);
		default:
			unexpectedChar(curOffset);
	}
L_1082b8:
	switch(charAt(curOffset))
	{
		case ATEOF:
		default:
			advanceTo(curOffset);
			setFlags(0x0);
			return TOKEN_DOTDOTDOT;
	}

