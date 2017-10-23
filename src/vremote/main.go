/* ######################################################################
# Author: (zfly1207@126.com)
# Created Time: 2017-10-20 09:37:45
# File Name: main.go
# Description: 
####################################################################### */

package main

import (
	"io"
	"fmt"
	"os"
	"log"
	"strings"
	"net"
	"net/http"

	"github.com/go-vgo/robotgo"
)

func main() {

	ips := getIps()
	fmt.Println("=-=-=-==-=-=-=-=-=-=-=-=-==-=-=-=-")
	fmt.Println("Controller-PC start...\n访问地址：")
	for _, ip := range ips {
		fmt.Println(fmt.Sprintf("\thttp://%s:9090", ip))
	}
	fmt.Println("=-=-=-==-=-=-=-=-=-=-=-=-==-=-=-=-")

	http.HandleFunc("/", pageHandle)
	http.HandleFunc("/api", ajaxHandle)
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}

func getIps() (r []string) {
	addrs, err := net.InterfaceAddrs()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	for _, address := range addrs {
		// 检查ip地址判断是否回环地址
		if ipnet, ok := address.(*net.IPNet); ok && !ipnet.IP.IsLoopback() {
			if ipnet.IP.To4() != nil {
				r = append(r, ipnet.IP.String())
			}
		}
	}
	return
}

func ajaxHandle(w http.ResponseWriter, r *http.Request) {

	r.ParseForm()
	// fmt.Println("收到客户端请求: ", r.Form)

	keys := strings.Split(r.FormValue("k"), "+")
	fmt.Println("received key: ", keys)
	fmt.Println("-", keys[0])
	fmt.Println("--", keys[1:])

	if len(keys) == 0 {
		io.WriteString(w, `{"status":false}`)
		return
	} else if len(keys) == 1 {
		robotgo.KeyTap(keys[0])
	} else {
		robotgo.KeyTap(keys[0], keys[1:])
	}
	io.WriteString(w, `{"status":true}`)
}

func pageHandle(w http.ResponseWriter, r *http.Request) {
	html := `
<!doctype html>
<head>
	<title>VREMOTE</title>
	<meta name="viewport" content="width=device-width,initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0, user-scalable=1" />
	<style>
		body, p{padding:0;margin:0;list-style:none;opacity:0;-webkit-transition:opacity 0.5s;font-size:32px;}
		.clear{clear:both;}
		.error{border:1px solid #a94442;color:#a94442;}
		ul, li{list-style:none;margin:0;padding:0;}
		label{font-weight:bold;}
		input{padding:0.3em 0.86em;height:1.5em;color:#555;border:1px solid #ccc;border-radius:0.5em;box-shadow:inset 0 1px 1px rgba(0, 0, 0, .075);transition:border-color ease-in-out .15s, box-shadow ease-in-out .15s;font-family:inherit;font-size:1em;}
		select{padding:0.3em 0.86em;height:2.3em;color:#555;border:1px solid #ccc;border-radius:0.5em;box-shadow:inset 0 1px 1px rgba(0, 0, 0, .075);transition:border-color ease-in-out .15s, box-shadow ease-in-out .15s;font-family:inherit;font-size:1em;}
		textarea{padding:0.3em 0.86em;line-height:1.5em;color:#555;border:1px solid #ccc;border-radius:0.5em;box-shadow:inset 0 1px 1px rgba(0, 0, 0, .075);transition:border-color ease-in-out .15s, box-shadow ease-in-out .15s;font-family:inherit;font-size:1em;}
		header{background:#e7e7e7;border-bottom:1px solid transparent;padding:0 0 0 0;}
		header li{height:2.5em;line-height:2.5em;padding:0 1.7em;}
		header li.left{float:left;}
		header li.item{cursor:pointer;}
		header li.item:hover {background:gray;color:white;}
		header li.item.open{background:gray;color:white;}
		header li.right{float:right;}
		header li.title span{font-size:1.7em;font-weight:bold;font-family:"楷体";color:#777;}

		#wrap{margin:1.5em auto 1em auto; width:19em;}
		#wrap .item{border:1px solid #e5e5e5;width:5em;margin:0.2em 0.66em;padding:0.5em 0;float:left;text-align:center;box-sizing:border-box;}
		#wrap .item.active{border:1px solid #555;color:#666;}
	</style>
	<script src="https://cdn.bootcss.com/jquery/3.1.1/jquery.min.js"></script>
	<script type="text/javascript">
		var buttons = [
			{'label' : '快退', 'key' : 'left', 'toggle-label' : ''},
			{'label' : '暂停', 'key' : 'space', 'toggle-label' : '开始'},
			{'label' : '快进', 'key' : 'right', 'toggle-label' : ''},
			{'label' : '音量+', 'key' : 'up', 'toggle-label' : ''},
			{'label' : '音量-', 'key' : 'down', 'toggle-label' : ''},
		]

		function resize(){
			var fontSize = (window.innerWidth/320)*16;
			document.body.style.fontSize = fontSize+'px';
		}

		function render(buttons){
			var html = [];
			for(var k in buttons) {
				html.push('<div class="item" data-key="'+ buttons[k]['key'] +'" data-toggle-label="'+ buttons[k]['toggle-label'] +'">'+ buttons[k]['label'] +'</div>');
			}
			$('#wrap').html(html.join(''));
		}

		$(document).ready(function(){
			resize();
			window.onresize = resize;
			document.body.style.opacity = '1';

			render(buttons);

			$('#wrap .item').on('click', function(){
				var node = $(this);
				$.ajax({
					'url' : '/api?k=' + $(this).data('key'),
					'dataType' : 'json',
					'success' : function(r){
						var toggleLabel = node.data('toggle-label');
						if (toggleLabel != '') {
							node.data('toggle-label', node.html()).html(toggleLabel);
						}
						console.log(r)
					},
					'error' : function(r) {
						alert('发生了错误' + r)
					}
				})
			})
		});
	</script>
</head>

<body>
	<header>
		<ul>
			<li class="left title"><span>vremote</span></li>
			<!--<li class="left item open"></li>-->
			<li class="right"></li>
			<div class="clear"></div>
		</ul>
	</header>

	<div id="wrap">
		<!--<div class="item active">快进</div>-->
	</div>
</body>
</html>
	`
	io.WriteString(w, html)
}

// vim: set noexpandtab ts=4 sts=4 sw=4 :
