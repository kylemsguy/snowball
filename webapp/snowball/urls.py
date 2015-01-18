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
    url('', include('social.apps.django_app.urls', namespace='social')),
    url(r'^$', 'winter.views.home', name='home'),
    url(r'^link_email$', 'winter.views.link_email', name='link_email'),
    url(r'^link_email_callback$', 'winter.views.link_email_callback', name='link_email_callback'),
    url(r'^signup$', 'winter.views.signup', name='link_email'),
    url(r'^testme$', 'winter.views.testme', name='testme'),
)
