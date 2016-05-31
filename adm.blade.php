<!DOCTYPE html>
<html>
    <head>
        <title>ADM {{ date("Y") }}-{{ date("Y") + 1 }}</title>

<link href='https://fonts.googleapis.com/css?family=Lobster&subset=latin,cyrillic' rel='stylesheet' type='text/css'>
<link href='https://fonts.googleapis.com/css?family=Comfortaa:700&subset=latin,cyrillic' rel='stylesheet' type='text/css'>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" integrity="sha384-1q8mTJOASx8j1Au+a5WDVnPi2lkFfwwEAa8hDDdjZlpLegxhjVME1fgjWPGmkzs7" crossorigin="anonymous">

<script src="https://code.jquery.com/jquery-1.11.3.js"></script>

        <style>
            html, body {
                height: 100%;
                background-image: url(img/olivie_tile.jpeg);
                background-position: left bottom;
                background-repeat: repeat-x;
            }

            body {
                margin: 0;
                padding: 0;
                width: 100%;
                display: table;
            	font-family: 'Lobster', cursive;
                font-size: 2em;
            }

            .container {
                text-align: center;
                display: table-cell;
            }

            .content {
                background-color: white;
                padding: 40px;
                text-align: center;
                display: inline-block;
            }

            .title {
                font-family: 'Comfortaa', cursive;
                font-weight: 700;
                font-size: 80px;
                color: #3697FF;
                opacity: 100%;
                text-shadow: 2px 2px #ffFFFF;
            }

            .ui {
                text-align: left;
                display: inline-block;
            }

            .notice {
            	display: none;
            }

            .red {
            	color: red;
            }

            .green {
            	color: green;
            }

            .white {
            	color: white;
            }

            table {
                background-color: white;
                opacity: 1.0;
            }

            th {
            	padding-left: 10px;
            }

            .btncheck {
            	cursor: pointer;
            }

            .btnremove {
            	cursor: pointer;
            }

        </style>

        <script type="text/javascript">

    var Parties = window.Parties = {
        id: 0,
        ppp: [],
        rules: null
    };

    Parties.PrintRules = function () {

    	if(this.rules == null)
    		return "";

    	var table = $('<table />').addClass('table table-striped table-bordered');

    	var TH = "<th>Имя</th>";
    	for( x = 0; x < this.ppp.length; x++)
    	{
    		TH += "<th title='" + this.ppp[x][1] + "'>" + x + "</th>";
    	}
   		TH += "<th></th>";
    	table.append(TH);

    	for(var x = 0; x < this.ppp.length; x++)
    	{
    		var ROW = "<tr><td title='" + this.ppp[x][2] + "'>" + this.ppp[x][1] + "</td>";

    		for(var rx = 0; rx < this.ppp.length; rx++)
    		{
    			ROW += "<td>" + (this.rules[x][rx] == true
    			 ? "<span coord='" + x + "." + rx + "' class='btncheck glyphicon glyphicon-ok green' title='" + this.ppp[x][1] + " может дарить " + this.ppp[rx][1] + "'>" 
    			 : "<span coord='" + x + "." + rx + "' class='btncheck glyphicon glyphicon-minus white'>") + "</td>"
    		}

    		ROW += "<td><span id='" + x + "' class='glyphicon glyphicon-remove red btnremove' title='Вообще не будет'></span></td></tr>";
	    	table.append(ROW);
    	}

    	return table;
    };

    Parties.RebuildRules = function () {

    	var vsize = this.ppp.length;
    	if(vsize == 0)
    		return this.PrintRules();

    	if(this.rules == null) {
    		this.rules = [[true]];
    		return this.PrintRules();
    	}

    	for (var x = 0; x < vsize - 1; x++) {
    		this.rules[x].push(true);
    	}

    	this.rules.push([]);
    	for (var x = 0; x < vsize; x++) {
    		this.rules[vsize - 1].push(true);
    	}

    	return this.PrintRules();
    };

    Parties.TriggerRule = function (x, y) {
    	if(x >= this.ppp.length || y >= this.ppp.length)
    		return;

    	this.rules[x][y] = !this.rules[x][y];
        if(x != y)
        {
            this.rules[y][x] = !this.rules[y][x];
        }
    	return this.PrintRules();
    };

    Parties.addParty = function (pname, pemail) {
        pname = pname.trim();
        pemail = pemail.trim();
        if (pname.length > 0) {
	        this.ppp.push([this.id++, pname, pemail]);
	        return this.RebuildRules()
        }

        return "";
    };

    Parties.RemovePart = function (idx) {
    	if(idx >= this.ppp.length)
    		return;

    	var vsize = this.ppp.length;
    	this.ppp.splice(idx, 1);

    	if(this.ppp.length == 0)
    	{
    		this.rules = null;
    		return "";
    	}

    	for (var x = 0; x < vsize; x++) {
    		this.rules[x].splice(idx, 1);
    	}

    	this.rules.splice(idx, 1);

    	return this.PrintRules();
    };

    Parties.RecursiveGenerate = function (idx_next, morph, res) {
    	var vIdx = [];
		for (var y = 0; y < this.ppp.length; y++)
		{
			if (this.rules[idx_next][morph[y]] == true && $.inArray(morph[y], res) < 0)
				vIdx.push(morph[y]);
		}

		if (vIdx.length == 0)
		{
			if (res.length != this.ppp.length - 1)
			{
				return [false, res];
			}
			else
			{
				res.push(idx_next);
				return [true, res];
			}
		}

		for (var x = 0; x < vIdx.length; x++)
		{
			res.push(idx_next);
			var rPair = this.RecursiveGenerate(vIdx[x], morph, res);
			if (rPair[0] == true)
			{
				return [true, rPair[1]];
			}
			else
			{
				res.pop();
			}
		}

		return [false, res];
    };

    Parties.CheckGraph = function () {
		for (var x = 0; x < this.ppp.length; x++)
		{
			var entries = 0;
			for (var y = 0; y < this.ppp.length; y++)
			{
				if (this.rules[x][y] == true)
					entries++;
			}

			if(entries < 2)
				return false;
		}

		return true;
	}

    Parties.Generate = function () {
		if (this.ppp.length == 0 || this.ppp.length < 3)
			return false;

		if(!this.CheckGraph())
			return false;

		shuffle(this.ppp);
		var morph = new Array(this.ppp.length);
		for (var i = 0; i < this.ppp.length; i++)
		{
			morph[this.ppp[i][0]] = i;
		}

		var ret = this.RecursiveGenerate(morph[0], morph, []);

		if (!ret[0] || ret[1].length == 0 || ret[1].length != this.ppp.length)
			return false;

		var res = [];
		for (var i = 0; i < this.ppp.length - 1; i++)
		{
			res.push( [ this.ppp[morph[ret[1][i]]], this.ppp[morph[ret[1][i + 1]]] ] );
		}

		res.push([ this.ppp[ morph[ret[1][ret[1].length - 1]]], this.ppp[morph[ret[1][0]]]]);

		this.ppp = [];
		this.rules = null;
		this.id = 0;

		return res;
    };

function shuffle(o){
    for(var j, x, i = o.length; i; j = Math.floor(Math.random() * i), x = o[--i], o[i] = o[j], o[j] = x);
    return o;
}

function CheckClick () {
	var x = $(this).attr('coord');
	var cc = x.split('.');
	var ph = Parties.TriggerRule(cc[0], cc[1]);
	$('.parties').html(ph);
	$('.btncheck').click(CheckClick);
	$('.btnremove').click(CheckRemove);
}

function CheckRemove () {
	var ph = Parties.RemovePart($(this).attr('id'));
	$('.parties').html(ph);
	$('.btncheck').click(CheckClick);
	$('.btnremove').click(CheckRemove);
}

$(window).load(function () {

    $('.ok').click(function () {
        var input_name = $('.newOne #pname');
        var input_email = $('.newOne #pemail');
        var ph = Parties.addParty(input_name.val(), input_email.val());
		$('.parties').html(ph);
		$('.btncheck').click(CheckClick);
		$('.btnremove').click(CheckRemove);

        input_name.val('');
        input_email.val('');
        input_name.focus();
    });

    $('.runRaffle').click(function () {
        var results = Parties.Generate();
        if (!results) {
            $('.notice').show();
            return;
        }

        $('.notice').hide();
//		$('.parties').html("");
        for (var i = 0; i < results.length; i++) {
			// send mail
			var data = {
				sendername: results[i][0][1],
				sendermail: results[i][0][2],
				receivername: results[i][1][1],
				_token: '{{ csrf_token() }}'
			};

			$.post('{{ url("adm") }}', data, function(msg) {}, 'json');
        }
		$('.bg-info').html("Вроде все отправили. Вперед! Проверять почту!");


    });

});

        </script>

    </head>

    <body>
        <div class="container">
            <div class="content">
                <div class="title"><b>Анонимный Дед Мороз<br>{{ date("Y") }}-{{ date("Y") + 1 }}</b></div>
                <div>
                	<h1>Участники</h1>
                        <div class="ui">
                     	   <div class="parties"></div>
	                        <p class="newOne"><span class="lang ru">Добавить участника</span><br>
	                        	Имя: <input type="text" id="pname"/>
	                        	Email: <input type="text" id="pemail"/>
	                        	<input type="button" class="ok" value="OK" /></p>
	                        <div class="notice bg-danger">Не получилось создать розыгрыш. Добавьте участника или измените правила получения</div>
                    	</div>
                        <div><br><br><button type="button" class="btn btn-primary btn-lg btn-block runRaffle">Поехали!</button></div>
                        <div class="bg-info"></div>
                </div>
            </div>
        </div>
    </body>

</html>
