// Alarm Set UI Script

var schedule = {}

$(document).ready(function () {
	// Let's create the option menus
	for (var i = 0; i <= 23; i++) {
		var hour;
		if (i < 10) {
			hour = "0" + i;
		} else {
			hour = i;
		}
		$("#start_hour").append($("<option>").attr("value", hour).text(hour));
		$("#end_hour").append($("<option>").attr("value", hour).text(hour));
	}

	for (var i = 0; i <= 59; i++) {
		var minutes;
		if (i < 10) {
			minutes = "0" + i;
		} else {
			minutes = i;
		}
		$("#start_minute").append($("<option>").attr("value", minutes).text(minutes));
		$("#end_minute").append($("<option>").attr("value", minutes).text(minutes));
		//$("#on_duration").append($("<option>").attr("value", minutes).text(minutes));
		//$("#off_duration").append($("<option>").attr("value", minutes).text(minutes));
	}
	for (var i = 5; i <= 180; i+=5) {
		var minutes;
		if (i < 10) {
			minutes = "0" + i;
		} else {
			minutes = i;
		}
		$("#on_duration").append($("<option>").attr("value", minutes).text(minutes));
		$("#off_duration").append($("<option>").attr("value", minutes).text(minutes));
	}

	// Let's set default values
	var s_time;
	var e_time;
	var on_dur;
	var off_dur;

	$.ajax({
		type: "GET",
		dataType: "json",
		url: "scheduler",
		timeout: 5000
	}).done(function (data) {
		s_time = data.s_time;
		e_time = data.e_time;
		on_dur = data.on_dur;
		off_dur = data.off_dur;
		

		if (s_time != undefined) {
			tmp = s_time.split(":");
			$("#start_hour option[value=" + tmp[0] + "]").prop("selected", true);
			$("#start_minute option[value=" + tmp[1] + "]").prop("selected", true);
		}
		if (e_time != undefined) {
			tmp = e_time.split(":");
			$("#end_hour option[value=" + tmp[0] + "]").prop("selected", true);
			$("#end_minute option[value=" + tmp[1] + "]").prop("selected", true);			
		}

		if (on_dur != undefined) {
			$("#on_duration option[value=" + on_dur + "]").prop("selected", true);
		}				
		if (off_dur != undefined) {
			$("#off_duration option[value=" + off_dur + "]").prop("selected", true);
		}
		
		//if (data.alarm.active == "true") {
		if (s_time != undefined) {
			$("#btn_on").prop("value", "true");
			$("#btn_off").prop("value", "false");
			$("#btn_on").addClass("active");
			$("#btn_off").addClass("btn-default");
			$("#btn_on").addClass("btn-primary");
		} else {
			$("#btn_on").prop("value", "false");
			$("#btn_off").prop("value", "true");
			$("#btn_off").addClass("active");
			$("#btn_on").addClass("btn-default");
			$("#btn_off").addClass("btn-primary");
		}
	});

	$(".saveButton").on("click", function () {
		
		var active = $("#btn_on").prop("value");
		var s_time = document.getElementById("start_hour").value + ":" + document.getElementById("start_minute").value;
		var e_time = document.getElementById("end_hour").value + ":" + document.getElementById("end_minute").value;
		var on_dur = document.getElementById("on_duration").value;
		var off_dur = document.getElementById("off_duration").value;
		
		schedule["active"] = active;
		schedule["s_time"] = s_time;
		schedule["e_time"] = e_time;
		schedule["on_dur"] = on_dur;
		schedule["off_dur"] = off_dur;
		postSchData(schedule);
		
		//$.get("/configSchedule?&active=" + active + "&s_time=" + s_time + "&e_time=" + e_time 
		//		+ "&on_dur=" + on_dur+ "&off_dur=" + off_dur ,function(data,status){
		//	alert("ON/OFF Schedule saved!");
		//	console.log("Done!");
		//});
		return false;
	});


	$('.btn.btn-sch').click(function () {
		$('.btn.btn-sch').toggleClass('active');
		$('.btn.btn-sch').toggleClass('btn-default');
		$('.btn.btn-sch').toggleClass('btn-primary');
		if ($(this).html() == "ON") {
			$("#btn_on").prop("value", "true");
			$("#btn_off").prop("value", "false");
		} else {
			$("#btn_on").prop("value", "false");
			$("#btn_off").prop("value", "true");
		}
	});
});

function postSchData(t) {
	var e = new XMLHttpRequest;
	e.timeout = 5000;
	e.open("PUT", "/scheduler", !0);
	e.setRequestHeader("Content-Type", "application/json");
	e.onreadystatechange = function() { // Call a function when the state changes.
		if (this.readyState === XMLHttpRequest.DONE && this.status === 200) {
			// Request finished. Do processing here.
			alert("ON/OFF Schedule saved!");
			//console.log(JSON.stringify(t));
		}
	}
	e.send(JSON.stringify(t))
	console.log(JSON.stringify(t));		
}

var state = {}

function updateStatus() {
  $.ajax({
    type: 'GET',
    url: "ac_state",
    dataType: "json",
    data: [ ],
    success: function(data) {
      if (!data) {
        return;
      }
      state = data;
      if (state["power"] === true) {
        $("#power").text(" ON");
        $("#power-btn").addClass("btn-info");
        $("#power-btn").removeClass("btn-default");
      } else {
        $("#power").text(" OFF");
        $("#power-btn").addClass("btn-default");
        $("#power-btn").removeClass("btn-info");
      }
      $("#target_temp").text(state["temp"] + " C");
      setModeColor(state["mode"]);
      setFanColor(state["fan"]);
    },
    error: function() {
      console.log('error getting state');
    },
    timeout: 5000
  });
}

updateStatus();




function postData(t) {
  var e = new XMLHttpRequest;
  e.timeout = 5000;
  e.open("PUT", "ac_state", !0);
  e.setRequestHeader("Content-Type", "application/json");
  console.log(JSON.stringify(t)), e.send(JSON.stringify(t));
}

function mode_onclick(mode) {
  state["mode"] = mode;
  setModeColor(mode);
  postData(state);
}


function setModeColor(mode) {
  $(".mode-btn").addClass("btn-default");
  $(".mode-btn").removeClass("btn-info");

  if (mode === 0) {
    $("#mode_auto").removeClass("btn-default");
    $("#mode_auto").addClass("btn-info");
    setFanColor(0);
    state["fan"] = 0;
  } else if (mode === 1) {
    $("#mode_cooling").removeClass("btn-default");
    $("#mode_cooling").addClass("btn-info");
  } else if (mode === 2) {
    $("#mode_dehum").removeClass("btn-default");
    $("#mode_dehum").addClass("btn-info");
  } else if (mode === 3) {
    $("#mode_heating").removeClass("btn-default");
    $("#mode_heating").addClass("btn-info");
  } else if (mode === 4) {
    $("#mode_fan").removeClass("btn-default");
    $("#mode_fan").addClass("btn-info");
  }
}

function setFanColor(fan) {
  if (fan == 0) {
    $("#fan_auto").removeClass("btn-default");
    $("#fan_auto").addClass("btn-info");
  } else {
    $("#fan_auto").removeClass("btn-info");
    $("#fan_auto").addClass("btn-default");
  }
  for (var i = 1; i <= 3; ++i) {
    if (i <= fan) {
      $("#fan_lvl_" + i).attr("src", "level_" + i + "_on.svg");
    } else {
      $("#fan_lvl_" + i).attr("src", "level_" + i + "_off.svg");
    }
  }
}

function fan_onclick(fan) {
  if (state["mode"] !== 0) {
    state["fan"] = fan;
    setFanColor(fan);
    postData(state);
  }
}

function power_onclick(power) {
  if (state["power"]) {
    state["power"] = false;
    $("#power").text(" OFF");
    $("#power-btn").removeClass("btn-info");
    $("#power-btn").addClass("btn-default");
  } else {
    state["power"] = true;
    $("#power").text(" ON");
    $("#power-btn").addClass("btn-info");
    $("#power-btn").removeClass("btn-default");
  }
  postData(state);
}


function temp_onclick(temp) {
  state["temp"] += temp;
  if (state["temp"] < 17) {
    state["temp"] = 17;
  }
  if (state["temp"] > 30) {
    state["temp"] = 30;
  }
  $("#target_temp").text(state["temp"] + " C");
  postData(state);
}
