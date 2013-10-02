% sets = include('ololo.tpl')

% ASSET_PATH = sets['ASSET_PATH']
% HOME_PATH = sets['HOME_PATH']
% BASE_PATH = sets['BASE_PATH']

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <title>{{ title }}</title>
    <meta name="author" content="Oleg Baskakov"/>

    <!-- Enable responsive viewport -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>

    <!-- Le styles, bootstrap main css -->
    <link href="{{ ASSET_PATH }}/bootstrap.css" rel="stylesheet"/>
    <link href="{{ ASSET_PATH }}/style.css?body=1" rel="stylesheet" type="text/css" media="all"/>
  </head>


  <body>
    <div class="navbar navbar-static-top">
      <div class="navbar-inner">
        <a class="brand" href="{{ HOME_PATH }}">{{ title }}</a>
        <div class="container">
        <ul class="nav">
            <li class="active">
            <a href="{{ BASE_PATH }}{{ url }}" class="active">ALL BOOKS</a>
            </li>
        </ul>
        </div>    
     </div>
   </div>

