
//����� FIXED - �᫮ � 䨪�஢����� ����⮩
class FIXED
{
	long i;
public:
	FIXED() {}	//���������
	FIXED(float n)
	{
		i=n*256;
	}
	operator int ()
	{
		return i>>8;
	}
	void operator += (FIXED f)
	{
		i+=f.i;
	}
};

//����� FLOAT - �᫮ � 䨪�஢����� ����⮩
class FLOAT
{
	long i;
public:
	FLOAT() {}	//���������
	FLOAT(float n)
	{
		i=n*256;
	}
	operator int ()
	{
		return i>>8;
	}
	void operator += (FLOAT f)
	{
		i+=f.i;
	}
	void operator -= (FLOAT f)
	{
		i-=f.i;
	}

	void operator -= (int f)
	{
		i-=f*256;
	}

	void operator *= (int n)
	{
		i*=n;
	}

	int operator > (int n)
	{
		if ( (i>>8) > n ) return 1;
		else return 0;
	}

	int operator < (int n)
	{
		if ( (i>>8) < n ) return 1;
		else return 0;
	}

	int operator == (int n)
	{
		if ( (i>>8) == n ) return 1;
		else return 0;
	}

	int operator >= (int n)
	{
		if ( (i>>8) >= n ) return 1;
		else return 0;
	}

	int operator <= (int n)
	{
		if ( (i>>8) <= n ) return 1;
		else return 0;
	}

};