//
// I obtained this from Google. No idea what it does.
//

        blogFeedUrl = 'https://ajax.googleapis.com/ajax/services/feed/load?v=1.0&q=' +
          'http://www.blogger.com/feeds/6781693/posts/default&num=1&callback=loadBlogFeed';
        function loadBlogFeed(r){
          var container = document.getElementById('feed');
          if (r.responseStatus == '200' && r.responseData.feed.entries.length > 0) {
          var post = r.responseData.feed.entries[0];
          var title = unescapePureXMLEntities(post.title);
          var entry = unescapePureXMLEntities(cleanPostContent(post.content));
          var link = post.link;
          var date = post.publishedDate;
          date = new Date(date).toDateString();
          renderPost(title, entry, link, date);
          } else {
          renderNoPost();
          }
        }
        function cleanPostContent(entry) {
          entry = entry.replace(/<span>[^<]*<\/span>/, '');
          entry = entry.replace(/<[^>]*>/g, '');
          var snippet = entry.split(' ', 20);
          snippet.pop();
          return snippet.join(' ') + ' ...';
        }
        function unescapePureXMLEntities(str) {
          return str.replace(/&([^;]+);/g, function(s, entity) {
          switch (entity) {
          case 'amp':
          return '&';
          case 'lt':
          return '<';
              case 'gt':
                return '>';
          case 'quot':
          return '"';
          default:
          if (entity.charAt(0) == '#') {
          var n = Number('0' + entity.substr(1));
          if (!isNaN(n)){
          return String.fromCharCode(n);
          }
          }
          // For invalid entities we just return the entity
          return s;
          }
          });
        }
        function renderPost(title, entry, link, date) {
          var linkNode = document.createElement('a');
          linkNode.href = link;
          linkNode.appendChild(document.createTextNode(title));
          var snippetDiv = document.createElement('div');
          snippetDiv.className = 'snippet';
          snippetDiv.appendChild(document.createTextNode(entry));
          var dateDiv = document.createElement('div');
          dateDiv.className = 'time';
          dateDiv.appendChild(document.createTextNode(date));
          var container = document.getElementById('feed');
          container.appendChild(linkNode);
          container.appendChild(dateDiv);
          container.appendChild(snippetDiv);
        }
        function renderNoPost(){
          var container = document.getElementById('feed');
          container.innerHTML = '<a href="http://gmailblog.blogspot.com">' +
          'The Official Gmail Blog</a>';
        }
        function importBlogJS() {
          var scriptNode = document.createElement('script');
          scriptNode.setAttribute('src', blogFeedUrl);
          document.getElementsByTagName('head')[0].appendChild(scriptNode);
        }
        