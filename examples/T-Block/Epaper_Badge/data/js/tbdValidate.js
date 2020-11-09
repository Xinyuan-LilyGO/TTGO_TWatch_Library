//date: 2016-11-16 author: dgx note:必填正则验证工具  
function tbdValidate(elelist, objs) {
	var errlen = 0;//错误个数
	var errlist = [];//错误输入框ID
	for (var i = 0; i < elelist.length; i++) {	//失去焦点操作
		(function (eleinput, eletext, rule) {
			document.getElementById(eleinput).onblur = function () {
				var intval = this.value.replace(/(^\s*)|(\s*$)/g, "");
				for (var j = 0; j < rule.length; j++) {
					var resreg = rule[j].reg.test(intval);
					if (resreg) {
						document.getElementById(eletext).style.display = "none";
					} else {
						document.getElementById(eletext).innerHTML = rule[j].text;
						document.getElementById(eletext).style.display = "block";
						break;
					};
				};
			};
		})(elelist[i].eleinput, elelist[i].eletext, elelist[i].rule);
	};
	document.getElementById(objs.elesubmit).onclick = function () {//提交操作
		errlen = 0;
		errlist = [];
		for (var i = 0; i < elelist.length; i++) {
			(function (eleinput, eletext, rule) {
				var intval = document.getElementById(eleinput).value.replace(/(^\s*)|(\s*$)/g, "");
				for (var j = 0; j < rule.length; j++) {
					var resreg = rule[j].reg.test(intval);
					if (resreg) {
						document.getElementById(eletext).style.display = "none";
					} else {
						document.getElementById(eletext).innerHTML = rule[j].text;
						document.getElementById(eletext).style.display = "block";
						errlen += 1;
						errlist.push(eleinput);
						break;
					};
				};
			})(elelist[i].eleinput, elelist[i].eletext, elelist[i].rule);
		};
		if (errlen == 0) {//回调
			objs.funsubmit();
		} else {
			objs.funerr();
			objs.funerrlist(errlist);
		};
	};
};
//date: 2016-11-17 author: dgx note:非必填正则验证工具  
function tbdnoValidate(elelist) {
	for (var i = 0; i < elelist.length; i++) {	//失去焦点操作
		(function (eleinput, eletext, rule) {
			document.getElementById(eleinput).onblur = function () {
				var intval = this.value.replace(/(^\s*)|(\s*$)/g, "");
				for (var j = 0; j < rule.length; j++) {
					var resreg = rule[j].reg.test(intval);
					if (resreg) {
						document.getElementById(eletext).style.display = "none";
					} else {
						document.getElementById(eletext).innerHTML = rule[j].text;
						document.getElementById(eletext).style.display = "block";
						this.value = "";
						break;
					};
				};
			};
		})(elelist[i].eleinput, elelist[i].eletext, elelist[i].rule);
	};
};