addEventListener('fetch', event => {
    event.respondWith(handleRequest(event.request))
  })
  
  async function handleRequest(request) {
    const url = new URL(request.url)
  
    // Pattern match for extensionless URLs
    const isLikelyJson = !url.pathname.includes('.')  // no dot = no extension
  
    // GitHub Pages backend URL (you can tweak this to your actual subdomain if needed)
    const githubOrigin = `https://blue.inz.com.ar`
    const githubUrl = githubOrigin + url.pathname
  
    const response = await fetch(githubUrl, request)
  
    // Clone headers
    const newHeaders = new Headers(response.headers)
  
    // Rewrite headers if it's likely JSON
    if (isLikelyJson) {
      newHeaders.set('Content-Type', 'application/json')
      newHeaders.delete('Content-Disposition') // in case GitHub sets it to force download
    }
  
    return new Response(response.body, {
      status: response.status,
      statusText: response.statusText,
      headers: newHeaders
    })
  }
  