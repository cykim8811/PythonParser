#include "Parser.h"
#include <iostream>

using namespace std;


int is_digit_0(string* source, int index) {
	if (source->at(index) >= '0' && source->at(index) <= '9')
		return index + 1;
	return -1;
}
int is_digit_1(string* source, int index) {
	if (source->at(index) >= '1' && source->at(index) <= '9')
		return index + 1;
	return -1;
}
int is_alphabet_a(string* source, int index) {
	if (source->at(index) >= 'a' && source->at(index) <= 'z')
		return index + 1;
	return -1;
}
int is_alphabet_A(string* source, int index) {
	if (source->at(index) >= 'A' && source->at(index) <= 'Z')
		return index + 1;
	return -1;
}

int is_string_sl_dq(string* source, int index) {
	if (source[index] == "\n" || source[index] == "\"") {
		return -1;
	}
	return index + 1;
}

Format F_INT(AND, {
	Format(FTN, is_digit_1),
	Format(REP, {Format(FTN, is_digit_0)})
	});
Format F_VAR(AND, {
	Format(OR, {
		Format(FTN, is_alphabet_a),
		Format(FTN, is_alphabet_A),
		Format("_")
	}),
	Format(REP,{
		Format(OR,{
			Format(FTN, is_alphabet_a),
			Format(FTN, is_alphabet_A),
			Format(FTN, is_digit_0),
			Format("_")
		})
	})
	});

Format B(REP, { Format(" ") });
Format N(REP, { Format("\n") });
Format BN(REP, { Format(OR, {B, N}) });

S SB("ignore", B);
S SBN("ignore", BN);

S s_var("variable", AND, { S("address", F_VAR) });
S s_num("variable", AND, { S("number", F_INT) }); // TODO: Expand to other numbers; float, 0xblah ...
S s_list("variable", OR, {
	S("list", AND, {
		S("["), SBN, S("variable", addr), SBN, S("", REP,{
			S(","), SBN, S("variable", addr), SBN
		}),
		S("]")
	}),
	S("list", AND, {
		S("["), SBN,
		S("]")
	})
});
S s_tuple("variable", OR, {
	S("tuple", AND, {
		S("("), SBN, S("variable", addr), SBN, S("", REP,{
			S(","), SBN, S("variable", addr), SBN
		}),
		S(")")
	}),
	S("tuple", AND, {
		S("("), SBN,
		S(")")
	})
});
S s_string("variable", OR, {
	S("", AND, {
		S("\""),
		S("string", Format(REP, {Format(FTN, is_string_sl_dq)})),
		S("\"")
	})

});


S s_attr("attribute", AND, {
	S(".", "."),
	S("name", F_VAR)
});
S s_call("calling", AND, {
	S("", OR, {
		S("", AND, {
			S("(", "("),
			S("variable", addr), S("", REP, {
				SB, S(",", ","), SBN, S("variable", addr)
			}),
			S(")", ")")
		}),
		S("", AND, {
			S("(", "("),
			S(")", ")")
		})
	})
});
S s_index("indexing", AND, {
	S("[", "["),
	S("variable", addr), S("", REP, {
		SB, S(",", ","), SBN, S("variable", addr)
	}),
	S("]", "]")
});

Node single_addr(string* source, int* index) {
	int ind;
	Node target;
	// From variable list
	ind = *index;
	target = s_var.fit(source, &ind);
	if (ind != -1) {
		*index = ind;
		return target;
	}
	// Number
	ind = *index;
	target = s_num.fit(source, &ind);
	if (ind != -1) {
		*index = ind;
		return target;
	}
	// List
	ind = *index;
	target = s_list.fit(source, &ind);
	if (ind != -1) {
		*index = ind;
		return target;
	}
	// Tuple(with brackets)
	ind = *index;
	target = s_tuple.fit(source, &ind);
	if (ind != -1) {
		*index = ind;
		return target;
	}
	// String
	ind = *index;
	target = s_string.fit(source, &ind);
	if (ind != -1) {
		*index = ind;
		return target;
	}
	*index = -1;
	return Node();
}

Node addr(string* source, int* index) {
	Node target = single_addr(source, index);
	if (*index == -1) {
		return Node();
	}

	while (true) {
		Node r;
		int ind;
		// attribute
		ind = *index;
		r = s_attr.fit(source, &ind);
		if (ind != -1) {
			*index = ind;
			r.subnode.insert(r.subnode.begin(), target);
			target = r;
			continue;
		}
		// call
		ind = *index;
		r = s_call.fit(source, &ind);
		if (ind != -1) {
			*index = ind;
			r.subnode.insert(r.subnode.begin(), target);
			target = r;
			continue;
		}
		// indexing
		ind = *index;
		r = s_index.fit(source, &ind);
		if (ind != -1) {
			*index = ind;
			r.subnode.insert(r.subnode.begin(), target);
			target = r;
			continue;
		}

		break;
	}
	return target;
}