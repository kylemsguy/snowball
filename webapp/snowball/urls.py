from django.conf.urls import patterns, include, url
from django.contrib import admin

from winter.views import *

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'snowball.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^callback', 'winter.views.contextio_callback'),
    url(r'^callback_fail', 'winter.views.contextio_failure_callback'),
)
