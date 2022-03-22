
from django.conf.urls import url 
from coordinates import views 
 
urlpatterns = [ 
    url(r'^api/coordinates$', views.handle_coordinates)]
    