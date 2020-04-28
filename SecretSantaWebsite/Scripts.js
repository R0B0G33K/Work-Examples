
	var nameC = 0;
	var Array = [];
	var Pairs = [];
	
function addName() {
	nameC++;
    var nform = document.getElementById("NameForm");
	var btn = document.getElementById("button");
	var t = document.createElement("Input");
	t.setAttribute("type","text");
    t.setAttribute("id","Mame" + nameC);
	
	var b = document.createElement("Br");
	nform.insertBefore(t, btn);
	nform.insertBefore(b, btn);
}

function subName() {
	if(nameC > 0){
	var nform = document.getElementById("NameForm");
	nform.removeChild(nform.childNodes[(nameC+1)*2]);    
	nform.removeChild(nform.childNodes[(nameC+1)*2]);
	nameC--;
	}
}

function next(){
	var jeff = 0;
	for (i = 0; i <= nameC; i++){
	 var name = document.getElementById("Mame"+i).value;
	 if (name == ""){
		jeff = 1;
		Array=[]; 
		alert("Uh Oh! Someone has been naughty! Did you type everyones name in?");
		break
	 }
	 else{
	 Array.push(name);
	 }
	 }
	 if (jeff == 0){
	 window.localStorage.setItem("data", JSON.stringify(Array));
	 document.location.href = "Relations.html";
	 }
}

/* Relations */

function relationLoad(){
	Array = JSON.parse(localStorage.getItem("data"));
    var list = document.getElementById("List");
	var dot;
	var elm;
	var check;
	var par;
	for (i = 0; i <= Array.length-1; i++){
		dot = document.createElement("li");
		elm = document.createTextNode(Array[i]+": ");
		dot.appendChild(elm);
		par = document.createElement("p");
		dot.appendChild(par);
		var	chunkAf = Array.slice(i+1,Array.length);
		var	chunkBf = Array.slice(0,i);
		if(i == 0){
			for  (p = 0; p < chunkAf.length; p++){
			check = document.createElement("INPUT");
			check.setAttribute("type", "checkbox");
			check.setAttribute("id", Array[i]+chunkAf[p]);
			checkname = document.createTextNode(" "+chunkAf[p]+".");
			par.appendChild(checkname);
			par.appendChild(check);
		}
		}
		else if (i == Array.length-1){
			for  (p = 0; p < chunkBf.length; p++){
			check = document.createElement("INPUT");
			check.setAttribute("type", "checkbox");
			check.setAttribute("id", Array[i]+chunkBf[p]);
			checkname = document.createTextNode(" "+chunkBf[p]+".");
			par.appendChild(checkname);
			par.appendChild(check);
		}
		}
		else{	
		for  (p = 0; p < chunkAf.length; p++){
			check = document.createElement("INPUT");
			check.setAttribute("type", "checkbox");
			check.setAttribute("id", Array[i]+chunkAf[p]);
			checkname = document.createTextNode(" "+chunkAf[p]+".");
			par.appendChild(checkname);
			par.appendChild(check);
		}
		for  (p = 0; p < chunkBf.length; p++){
			check = document.createElement("INPUT");
			check.setAttribute("type", "checkbox");
			check.setAttribute("id", Array[i]+chunkBf[p]);
			checkname = document.createTextNode(" "+chunkBf[p]+".");
			par.appendChild(checkname);
			par.appendChild(check);
		}
		}
		list.appendChild(dot);
	}
}


/* Radomize */

function randomize(){
	Array = JSON.parse(localStorage.getItem("data"));
    var max= Array.length;
	var constraints = [];
	

	start:
	while (true){
	for (i = 0; i <= Array.length-1; i++){
		var randomNum = Math.floor(Math.random() * (max)); 	
		var pairEval = Array[i]+ Array[randomNum];
		if (Array[i] == Array[randomNum]){
			Pairs = [];
			constraints = [];
			continue start;
		}
		else if (document.getElementById(pairEval).checked == true){
			Pairs = [];
			constraints = [];
			continue start;
		}
		else if (constraints.indexOf(Array[randomNum]) !== -1){
			Pairs = [];
			constraints = [];
			continue start;
		}
		else{
		constraints.push(Array[randomNum]);
		Pairs.push(Array[i]+ " has " + Array[randomNum]);
		}
	}
	break
	}
	window.localStorage.setItem("data2", JSON.stringify(Pairs));
	document.location.href = "End.html";
}

function endLoad(){
	Array = JSON.parse(localStorage.getItem("data"));
	Pairs = JSON.parse(localStorage.getItem("data2"));
	var endg = document.getElementById("endgame");
	for (i = 0; i <= Pairs.length-1; i++){
		var divv = document.createElement("div");
		divv.setAttribute("class","showhim");
		var divvv = document.createElement("div");
		divvv.setAttribute("class","showme");
		var divvvv = document.createElement("div")
		divvvv.setAttribute("class","ok");
		var p = document.createElement("p");
		var brr = document.createElement("br");
		var texxt = document.createTextNode(Pairs[i]);
		var texxxt = document.createTextNode(Array[i]);
		p.appendChild(texxt);
		divv.appendChild(texxxt);
		divvv.appendChild(p);
		divvvv.appendChild(brr);
		divv.appendChild(divvv);
		divv.appendChild(divvvv);
		endg.appendChild(divv);
	}
	
}

