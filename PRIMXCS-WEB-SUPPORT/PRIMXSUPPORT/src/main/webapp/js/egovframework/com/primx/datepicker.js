	/* ############datepicker############ */
	$(function(){

		$(".datepicker_wrap .date_wrap .date_txt_wrap").on("click",function(e){
	
			$(this).addClass("select");
			var this_ = $(this).parent(".date_wrap").attr("id");
			if(!$("#"+this_+" .picker_wrap").hasClass("open")){
				$(".picker_wrap").removeClass("open");
				$(".picker_wrap").html("");
				$("#"+this_+" .picker_wrap").addClass("open");
				date_pick_set($("#"+this_+" .picker_wrap"),e);
			}
		});
	
	});
	
	function date_pick_set(a, e) {
	
		var week = new Array("일", "월", "화", "수", "목", "금", "토");
		var mn = new Array(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
		var aaa = new Date();
	
		if ((a.parents(".date_wrap").children(".date_txt_wrap").text() != "클릭하세요")
				&& (a.parents(".date_wrap").children(".date_txt_wrap").text() != "")) {
	
			var htxt_ = a.parents(".date_wrap").children(".date_txt_wrap").text()
					.split(" ");
	
			if (htxt_.length == 1) {
				if (htxt_[0].split("-").length == 3) {
					htxt_.push("1:1");
				} else {
					htxt_.push(htxt_[0]);
					htxt_[0] = "2023-06-20";
				}
	
			}
	
			var htxt1 = htxt_[0].split("-");
			var htxt2 = htxt_[1].split(":");
			var hourtext = htxt_[1].split(":")[0];
			var mintext = htxt_[1].split(":")[1];
	
			aaa.setFullYear(Number(htxt1[0]));
			aaa.setMonth(Number(htxt1[1]) - 1);
			aaa.setDate(Number(htxt1[2]));
			aaa.setHours(hourtext);
			aaa.setMinutes(mintext);
	
			if (!Number(htxt1[0])) {
				aaa.setFullYear(1);
			}
			if (!Number(htxt1[1])) {
				aaa.setMonth(1);
			}
			if (!Number(htxt1[2])) {
				aaa.setDate(1);
			}
			if (!hourtext) {
				aaa.setHours(11);
			}
			if (!mintext) {
				aaa.setMinutes(11);
			}
	
		}
	
		var yy = aaa.getFullYear();
		var mm = aaa.getMonth() + 1;
		var dd = aaa.getDate();
		var hh = aaa.getHours();
		var min = aaa.getMinutes();
		var sec = aaa.getSeconds();
		var gd = aaa.getDay();
	
		
		var set_txt = "<div class='date_change_wrap'><em class='year_txt'></em><span class='today'>오늘</span><span class='arrow1'></span><span class='arrow2'></span><div class='date_wrap'><div class='week'><span>일</span><span>월</span><span>화</span><span>수</span><span>목</span><span>금</span><span>토</span></div><div class='num_wrap'></div><span class='bu_del'>삭제</span></div><div class='year_change hide'><div class='year_wrap'></div></div></div><div class='time_change_wrap'><div class='hour'><div class='wrap_'></div></div><div class='min'><div class='wrap_'></div></div></div>";
	
		a.html(set_txt);
	
	
		var time1 = "";
		for (var t1 = 0; t1 < 24; t1++) {
			t1 = t1 < 10 ? "0" + t1 : t1;
			time1 = time1 + "<span>" + t1 + "</span>";
		}
		a.find(".hour .wrap_").html(time1);
		var hh1 = hh;
		a.find(".hour .wrap_ span:eq(" + (hh) + ")").addClass("select");
		var htn = (hh - 4) < 0 ? 0 : (hh - 4) * 35;
		a.find(".hour").scrollTop(htn);
	
		var time2 = "";
		for (var t2 = 0; t2 < 60; t2++) {
			t2 = t2 < 10 ? "0" + t2 : t2;
			time2 = time2 + "<span>" + t2 + "</span>";
		}
		a.find(".min .wrap_").html(time2);
		a.find(".min .wrap_ span:eq(" + min + ")").addClass("select");
		var mtn = (min - 4) < 0 ? 0 : (min - 4) * 35;
		a.find(".min").scrollTop(mtn);
	
		a.find(".arrow2").click(function() {
			var dplus = Number(mn[aaa.getMonth()]) - Number(dd) + 2;
			aaa.setDate(Number(dd) + Number(dplus));
			year_change_open_set(a);
		})
	
		a.find(".arrow1").on("click", function() {
			var dplus = Number(dd);
			aaa.setDate(Number(dd) - Number(dplus));
			year_change_open_set(a);
		})
	
		a.find(".year_txt").on("click", function() {
			if (a.find(".year_change").hasClass("hide")) {
				a.find(".year_change").removeClass("hide");
				year_change_open(a, yy, 1982, 2050);
			}
		});
	
		a.find(".bu_del").on(
				"click",
				function() {
					a.removeClass("open");
					a.html("");
					a.parents(".date_wrap").children(".date_txt_wrap").text("");
					a.parents(".date_wrap").children(".date_txt_wrap").removeClass(
							"select");
					a.parents(".date_wrap").children(".date_txt_wrap").attr(
							"dateval", "");
				});
	
		a.find(".today").on("click", function() {
			aaa = new Date();
			yy = aaa.getFullYear();
			mm = aaa.getMonth() + 1;
			dd = aaa.getDate();
			hh = aaa.getHours();
			min = aaa.getMinutes();
			sec = aaa.getSeconds();
			gd = aaa.getDay();
	
			date_txt_add(a)
			year_change_open_set(a)
		});
	
		a.find(".hour span").on("click", function() {
			if ($(this).hasClass("select")) {
				var txt = hourtext
				aaa.setHours(txt);
			} else {
				var txt = Number($(this).text());
				a.find(".hour span").removeClass("select");
				$(this).addClass("select");
				aaa.setHours(txt);
			}
			date_txt_add(a)
		});
	
		a.find(".min span").on("click", function() {
			if ($(this).hasClass("select")) {
				var txt = mintext;
				aaa.setHours(txt);
			} else {
				var txt = Number($(this).text());
				a.find(".min span").removeClass("select");
				$(this).addClass("select");
				aaa.setMinutes(txt);
			}
			date_txt_add(a)
		});
	
	
		year_change_open_set(a);
	
		e.stopPropagation();
	
		var boxid = a.parent(".date_wrap").attr("id");
		window.addEventListener('click', function(e) {
			if (!document.getElementById(boxid).contains(e.target)) {
				a.removeClass("open");
				a.html("");
				a.parents(".date_wrap").children(".date_txt_wrap").removeClass(
						"select");
			}
		});
	
		function date_set(a, b, c, d, e, f, g) {
			a_ = a;
			b_ = b < 10 ? '0' + b : b;
			c_ = c;
			d_ = d < 10 ? '0' + d : d;
			e_ = e < 10 ? '0' + e : e;
			f_ = f < 10 ? '0' + f : f;
			g_ = g;
	
			if (g_ < (c_ - 1) % 7) { // 1일시작요일구하기
				var gd1 = (g_ + 7) - (c_ - 1) % 7;
			} else {
				var gd1 = g_ - (c_ - 1) % 7;
			}
			if (b_ - 2 < 0) { // 전달 시작날짜구하기
				var mm_ = (Number(b_) + 12) - 2;
			} else {
				var mm_ = Number(b_) - 2;
			}
	
			var bd = Number(mn[mm_]) - (gd1 - 1);
			var bdmx1 = Number(mn[mm_]) + 1;
			var bdmx2 = Number(mn[mm - 1]) + 1;
			if (Number(a_) % 4 == 0) {
				if (b == 2) {
					bdmx2++;
				} else if (b == 3) {
					bd++;
					bdmx1++;
				}
			}
			var wn = 1;
			var tn = 1;
			var dt = "";
			for (var i = bd; i < bdmx1; i++) {
				if (wn == 1) {
					dt = dt + "<div class='line'><span class='gray'>" + i
							+ "</span>";
				} else {
					dt = dt + "<span class='gray'>" + i + "</span>";
				}
				wn == 7 ? wn = 1 : wn++;
				tn++;
			}
			for (var i = 1; i < bdmx2; i++) {
				if (wn == 1) {
					dt = dt + "<div class='line'><span id='dd" + i + "'>" + i
							+ "</span>";
				} else if (wn == 7) {
					dt = dt + "<span id='dd" + i + "'>" + i + "</span></div>";
				} else {
					dt = dt + "<span id='dd" + i + "'>" + i + "</span>";
				}
				wn == 7 ? wn = 1 : wn++;
				tn++;
			}
	
			for (var i = 1; i < 44 - tn; i++) {
				if (wn == 1) {
					dt = dt + "<div class='line'><span class='gray'>" + i
							+ "</span>";
				} else if (wn == 7) {
					dt = dt + "<span class='gray'>" + i + "</span></div>";
				} else {
					dt = dt + "<span class='gray'>" + i + "</span>";
				}
				wn == 7 ? wn = 1 : wn++;
			}
			return dt;
		}
	
		function year_change_open(a, b, c, d) {
			var yi = "";
			for (var i = c; i < d + 1; i++) {
				yi = yi + "<div id='y" + i + "'><em>" + i + "</em></div>"
			}
	
			a.find(".year_wrap").html(yi);
	
			a.find(".year_wrap div").on("click", function() {
				var aa = $(this).attr("id");
				year_change_open_(a, aa);
			});
	
			year_change_open_(a, "y" + b);
	
			var stn = (b - c - 1) * 30;
			a.find(".year_change").scrollTop(stn)
		}
	
		function year_change_open_(a, b) {
			var aaaa = "<ul><li>1월</li><li>2월</li><li>3월</li><li>4월</li><li>5월</li><li>6월</li><li>7월</li><li>8월</li><li>9월</li><li>10월</li><li>11월</li><li>12월</li></ul>"
	
			if (!$("#" + b).hasClass("open")) {
				$("#" + b).addClass("open");
				$("#" + b).append(aaaa);
	
				$("#" + b + " li").on("click", function(e) {
					var a1 = $(this).parents("div").attr("id").slice(1);
					var a2 = $(this).html().slice(0, -1);
	
					aaa.setFullYear(a1);
					aaa.setMonth(a2 - 1);
					year_change_open_set(a);
	
					e.stopPropagation();
					a.find(".year_change").addClass("hide");
					a.find(".year_wrap").html("");
				});
			} else {
				$("#" + b).removeClass("open");
				var a1 = b.slice(1);
				$("#" + b).html("<em>" + a1 + "</em>");
			}
		}
	
		function year_change_open_set(a) {
	
			if (a.parents(".date_wrap").children(".date_txt_wrap").text() == "") {
				var ddd = new Date();
				var htxt = new Array(ddd.getFullYear(), ddd.getMonth() + 1, ddd
						.getDate());
			} else {
				var htxt_ = a.parents(".date_wrap").children(".date_txt_wrap")
						.text().split(" ");
				var htxt = htxt_[0].split("-");
			}
	
			var yy1 = htxt[0];
			var mm2 = htxt[1];
			var dd3 = htxt[2];
	
			yy = aaa.getFullYear();
			mm = aaa.getMonth() + 1;
			dd = aaa.getDate();
			gd = aaa.getDay();
	
			var ds = date_set(yy, mm, dd, hh, min, sec, gd);
			a.find(".year_txt").html(yy + "년 " + mm + "월");
			a.find(".num_wrap").html(ds);
	
			if (yy == yy1 && mm == mm2) {
				a.find(".num_wrap #dd" + Number(dd3)).addClass("select");
			}
	
			a.find(".num_wrap span").on("click", function() {
				if (!$(this).hasClass("gray")) {
					aaa.setDate($(this).text());
					a.find(".num_wrap span").removeClass("select");
					$(this).addClass("select");
					date_txt_add(a)
				}
			})
		}
	
		function date_txt_add(a) {
	
			a1 = aaa.getFullYear();
			b1 = aaa.getMonth() + 1;
			c1 = aaa.getDate();
			d1 = aaa.getHours();
			e1 = aaa.getMinutes();
			f1 = aaa.getSeconds();
			g1 = aaa.getDay();
	
			a_ = a1;
			b_ = b1 < 10 ? '0' + b1 : b1;
			c_ = c1 < 10 ? '0' + c1 : c1;
			d_ = d1 < 10 ? '0' + d1 : d1;
			e_ = e1 < 10 ? '0' + e1 : e1;
			f_ = f1 < 10 ? '0' + f1 : f1;
			g_ = g1;
	
			if (a.parents(".datepicker_wrap").hasClass("ddate")) {
				var t1 = a_ + "-" + b_ + "-" + c_;
				var t2 = a1 + "-" + b1 + "-" + c1;
			} else if (a.parents(".datepicker_wrap").hasClass("ttime")) {
				var t1 = d_ + ":" + e_;
				var t2 = d1 + ":" + e1;
				var t3 = d_ + ":" + e_;
			} else {
				var t1 = a_ + "-" + b_ + "-" + c_ + " " + d_ + ":" + e_;
				var t2 = a1 + "-" + b1 + "-" + c1 + " " + d1 + ":" + e1;
				var t3 = a_ + "-" + b_ + "-" + c_ + " " + d_ + ":" + e_;
			}
	
			a.parents(".date_wrap").children(".date_txt_wrap").text(t1);
			a.parents(".date_wrap").children(".date_txt_wrap").attr("dateval", t1);
	
			a.removeClass("open");
			a.html("");
			a.parents(".date_wrap").children(".date_txt_wrap")
					.removeClass("select");
		}
	
	}