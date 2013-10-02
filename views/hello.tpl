

% setdefault('title', 'Hello World!')
% setdefault('tagline', 'Supporting tagline')
% setdefault('posts', [])
% setdefault('paginator', {'page':1, 'total_pages': 2})

% sets = include('ololo.tpl')

% include("header.tpl", ololo="Page Title", title = "KOKO", url = '/erer/')

%import os.path

<div class="container">

<form action="/search" method="post">
    Find it: <input name="query" type="search" />
    <input value="Login" type="submit" />
</form>



<!-- start pagination -->
<div id="post-pagination" class="pagination">

  <ul class="pages">
    <li class="page">
    	% if paginator["page"] == 1:
        <span class="current-page">1</span>
    	% else:
        <a href="{{ sets['BASE_PATH'] }}/">1</a>
      	% end
    </li>

    % for count in range(2, paginator["total_pages"]):
      <li class="page">
        % if count == paginator["page"]:
          <span class="current-page">{{ count }}</span>
        % else:
          <a href="{{ sets['BASE_PATH'] }}/page{{ count }}">{{ count }}</a>
        % end
      </li>
    % end
  </ul>

</div>
<!-- end of pagination -->
<hr />

% for book in posts:
  <div class="media">
    <img src="{{ os.path.join('http://localhost/', book['book_path'], book.get('cover', '0.jpg')) }}"
         class="media-object pull-left"
         style="max-height: 300px;" />
    <div class="media-body">
      % for author in book.get('creator', []):
        <span class="badge badge-success pull-right">{{ author }}</span>
      % end
      <h2 class="media-heading"><a href="{{ sets['BASE_PATH'] }}/{{ book.get('url') }}">
      {{ book['title'] }}
      </a></h2>
      <p>{{ book.get('perface') }}</p>
    </div>
  </div>
  <hr />
% end

<div class="well"><a href="#">GO TO THE TOP -></a></div> 

</div>

% include("footer.tpl", ololo="Page Title", title = "KOKO", url = '/erer/')



