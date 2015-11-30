
template<typename Op, typename H>
bool CompareVariadicOr(const Op& o1, const H& op2)
{
	return o1 == op2;
}

template<typename Op, typename H, typename ...ARGS>
bool CompareVariadicOr(const Op& o1, const H& op2, ARGS... args)
{
	return o1 == op2 || CompareVariadicOr(o1, args...);
}

template<typename Op, typename ...ARGS>
bool CheckOr(const Op& o1, ARGS... args)
{
	bool bRes = CompareVariadicOr(o1, args...);

	return bRes;
}

template<typename Op, typename H>
bool CompareVariadicAnd(const Op& o1, const H& op2)
{
	return o1 == op2;
}

template<typename Op, typename H, typename ...ARGS>
bool CompareVariadicAnd(const Op& o1, const H& op2, ARGS... args)
{
	return o1 == op2 && CompareVariadicAnd(o1, args...);
}

template<typename Op, typename ...ARGS>
bool CheckAnd(const Op& o1, ARGS... args)
{
	bool bRes = CompareVariadicAnd(o1, args...);

	return bRes;
}
