--nota isaac: Els parametres "simples" surten a la especificació de l'XML? No estic molt segur que siguin estàndards.
--D'altra banda, no passa res, el codi el trobo molt correcte i eficaç.

function GetInitTagXML = 
(
	return "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>"
)

function OpenTagXML _Name =
(
	return "<" + _Name + ">"
)

function OpenTagParamXML _Name _Params =
(
	local l_Output = "<" + _Name
	
	if classof _Params == Array then
	(
		for i = 1 to _Params.count do
		(
			if ((classof _Params[i] == Array) and (_Params[i].count == 2)) then
			(
				l_Output = l_Output +" " + _Params[i][1] + "=\"" + _Params[i][2] + "\""
			)
			else if classof _Params[i] != Array then
			(
				l_Output = l_Output +" " + _Params[i]
			)
		)
	)
	
	l_Output = l_Output + ">"
	
	return l_Output
)

function CloseTagXML _Name =
(
	return "</" + _Name + ">"
)

function InlineTagXML _Name =
(
	return "<" + _Name + "/>"
)

function InlineTagParamXML _Name _Params =
(
	local l_Output = "<" + _Name
	
	if classof _Params == Array then
	(
		for i = 1 to _Params.count do
		(
			if ((classof _Params[i] == Array) and (_Params[i].count == 2)) then
			(
				l_Output = l_Output +" " + _Params[i][1] + "=\"" + _Params[i][2] + "\""
			)
			else if classof _Params[i] != Array then
			(
				l_Output = l_Output +" " + _Params[i]
			)
		)
	)
	
	l_Output = l_Output + "/>"
	
	return l_Output
)

/*
--Variables per fer proves
g_Nom = "Nom_del_Tag"
g_ParamSimple = "ParamSimple"
g_ParamValor = #("Param","Valor")
g_Params1 = #()
g_Params2 = #()
g_Params3 = #()
append g_Params1 g_ParamSimple
append g_Params2 g_ParamValor
append g_Params3 g_ParamValor
append g_Params3 g_ParamSimple
append g_Params3 g_ParamValor
--Fi Variables per fer proves

--Obrir tag sense paràmetres
OpenTagXML g_Nom

--Tag inline sense paràmetres
InlineTagXML g_Nom

--Tag inline amb paràmetres
--Paràmetre simple
InlineTagParamXML g_Nom g_Params1
--Paràmetre amb valor
InlineTagParamXML g_Nom g_Params2
--Composició de paràmetres amb valor i simples
InlineTagParamXML  g_Nom g_Params3

--Tancar tag
CloseTagXML g_Nom

--Obrir tag amb paràmetres
--Paràmetre simple
OpenTagParamXML g_Nom g_Params1
--Paràmetre amb valor
OpenTagParamXML g_Nom g_Params2
--Composició de paràmetres amb valor i simples
OpenTagParamXML g_Nom g_Params3

--Tancar tag
CloseTagXML g_Nom
--Tancar tag
CloseTagXML g_Nom
--Tancar tag
CloseTagXML g_Nom
*/