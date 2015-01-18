from django.shortcuts import render
from django.http import *
from django.views.decorators.csrf import *
from snowball.local_config import *
from django.views.decorators.http import *
from django.views.decorators.cache import never_cache
import social.apps.django_app.views
import contextio
import json
from winter.models import *
import datetime
import traceback

# Create your views here.

@csrf_exempt
def contextio_callback(request):
	try:
		print("We got it ")
		payload = json.loads(request.body)
		print(payload)
		user_profiles = UserProfile.objects.filter(context_io_account_id = payload["account_id"])
		if len(user_profiles) == 0:
			print("What? no profile?")
		else:
			user_profile = user_profiles[0]
			user = user_profile.user
			provider = user.social_auth.get(provider="google-oauth2")
			handle_callback(user, user_profile, provider, payload)
		return HttpResponse("Success")
	except BaseException as e:
		traceback.format_exc
		raise e

def handle_callback(user, user_profile, provider, payload):
	for body in payload["message_data"]["body"]:
		if body["type"] == "text/plain":
			break
	plain_text = body["content"]
	print(plain_text)

@csrf_exempt
def contextio_failure_callback(request):
	print("We failed ")
	payload = json.loads(request.body)
	return HttpResponse("Success")

def home(request):
	return render(request, 'winter/home.html', {})

def link_email(request):
	context_io = contextio.ContextIO(consumer_key = context_io_key, consumer_secret = context_io_secret)
	connect_token = context_io.post_connect_token(callback_url = server_addr + "/link_email_callback",
		email = request.session["school_email"], first_name = request.session["first_name"],
		last_name = request.session["last_name"])
	print(connect_token)
	if connect_token["success"]:
		return HttpResponseRedirect(connect_token["browser_redirect_url"])
	return HttpResponse("Failed to link email")

def link_email_callback(request):
	contextio_token = request.GET["contextio_token"]
	context_io = contextio.ContextIO(consumer_key = context_io_key, consumer_secret = context_io_secret)
	connect_token = contextio.ConnectToken(context_io, {"token": contextio_token})
	connect_token.get()
	new_profile = UserProfile(user=request.user, context_io_account_id=connect_token.account.id)
	new_profile.save()
	# attach webhook
	connect_token.account.post_webhook(callback_url=server_addr + "/callback",
		failure_notif_url=server_addr + "/callback_fail", sync_period="immediate", include_body=1)
	return render(request, 'winter/success.html', {})

@require_POST
def signup(request):
	request.backend = "google-oauth2"
	for a in ["school_email", "first_name", "last_name"]:
		request.session[a] = request.POST[a]

	return social.apps.django_app.views.auth(request, request.backend)
