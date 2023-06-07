;; A sum function that get 2 parameters
task[Int] add(a: Int, b: Int) {
	return a + b;
}

;; The Entry Function
task[Void] main() {
	;; y=Varname     <Int>=Vartype
	y<Int> = add(3,5);

	is  (y==8)? {
		print("y is 8");
	}
	|is (y == 9)? {
		print("y is 9");
	}
	else {
		print("y is $<y>");
	}

}

;; A structure like object
group user {
	;; Initializing the group
	task[Void] Init() {
		this.name<String>;
		this.age<Int>;
		this.id<Int>;
	}

	;; Set the name group variable
	task[Void] setName(name: String) {
		this.name = name;
	}
	
	;; Set the age group variable
	task[Void] setAge(age: Int) {
		this.age = age;
	}
	
	;; Set the id group variable
	task[Void] setId(id: Int) {
		this.id=id;
	}
}

;; Testing the group object
task[Void] groupTest() {
	user1[user] = user();
	user1::setName("Unity");
	user1::setAge(-1);
	user1::setId(55);
}