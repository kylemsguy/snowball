from django.shortcuts import render
from django.http import *
from django.views.decorators.csrf import *
from snowball.local_config import *
from django.views.decorators.http import *
from django.views.decorators.cache import never_cache
import social.apps.django_app.views
import contextio
import json

# Create your views here.

@csrf_exempt
def contextio_callback(request):
	print("We got it ")
	payload = json.loads(str(request.body, 'utf-8'))
	print(payload)
	return HttpResponse("Success")

@csrf_exempt
def contextio_failure_callback(request):
	print("We failed ")
	payload = json.loads(str(request.body, 'utf-8'))
	print(payload)
	return HttpResponse("Success")

def home(request):
	return render(request, 'winter/home.html', {})

def link_email(request):
	context_io = contextio.ContextIO(consumer_key = context_io_key, consumer_secret = context_io_secret)
	connect_token = context_io.post_connect_token(callback_url = server_addr + "/link_email_callback/" + str(request.user.id),
		email = request.session["school_email"], first_name = request.session["first_name"],
		last_name = request.session["last_name"])
	print(connect_token)
	if connect_token["success"]:
		return HttpResponseRedirect(connect_token["browser_redirect_url"])
	return HttpResponse("Failed to link email")

def link_email_callback(request):

	return render(request, 'winter/success.html', {})

@require_POST
def signup(request):
	request.backend = "google-oauth2"
	for a in ["school_email", "first_name", "last_name"]:
		request.session[a] = request.POST[a]

	return social.apps.django_app.views.auth(request, request.backend)
